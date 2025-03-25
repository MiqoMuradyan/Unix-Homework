#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <sys/mman.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <unistd.h>

#define SIZE 256

void* createSharedMemory() {
    void* sharedMemory = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (sharedMemory == MAP_FAILED) {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }
    return sharedMemory;
}

sem_t* createSemaphore(const std::string& name) {
    sem_t* semaphore = sem_open(name.c_str(), O_CREAT, 0644, 0);
    if (semaphore == SEM_FAILED) {
        perror("sem_open failed");
        exit(EXIT_FAILURE);
    }
    return semaphore;
}

void parentProcess(void* sharedMemory, sem_t* sem_parent, sem_t* sem_child) {
    char message[SIZE];
    while (true) {
        std::cout << "Parent: ";
        std::cin.getline(message, SIZE);
        std::strcpy(static_cast<char*>(sharedMemory), message);
        sem_post(sem_child);
        
        if (std::strcmp(message, "exit") == 0) return;

        sem_wait(sem_parent);
        std::cout << "Parent received: " << static_cast<char*>(sharedMemory) << std::endl;
    }
}

void childProcess(void* sharedMemory, sem_t* sem_parent, sem_t* sem_child) {
    char message[SIZE];
    while (true) {
        sem_wait(sem_child);
        std::cout << "Child received: " << static_cast<char*>(sharedMemory) << std::endl;
        
        std::cout << "Child: ";
        std::cin.getline(message, SIZE);
        std::strcpy(static_cast<char*>(sharedMemory), message);
        sem_post(sem_parent);
        
        if (std::strcmp(message, "exit") == 0) return;
    }
}

int main() {
    const std::string parentName = "/sem_parent";
    const std::string childName = "/sem_child";

    void* sharedMemory = createSharedMemory();
    sem_t* sem_parent = createSemaphore(parentName);
    sem_t* sem_child = createSemaphore(childName);

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork error");
        return EXIT_FAILURE;
    }
    else if (pid == 0) {
        childProcess(sharedMemory, sem_parent, sem_child);
        exit(EXIT_SUCCESS);
    }
    else {
        parentProcess(sharedMemory, sem_parent, sem_child);
        wait(NULL);
    }

    if (munmap(sharedMemory, SIZE) != 0) {
        perror("munmap failed");
    }
    sem_close(sem_parent);
    sem_close(sem_child);
    sem_unlink(parentName.c_str());
    sem_unlink(childName.c_str());

    return EXIT_SUCCESS;
}
