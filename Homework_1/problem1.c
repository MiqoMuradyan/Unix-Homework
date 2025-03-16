#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void rectangleArea() {
    float length;
    float width;
    printf("Input the params for a Rectangle\n");
    printf("Length: ");
    scanf("%f", &length);

    printf("Width: ");
    scanf("%f", &width);

    float area = length * width;
    printf("Rectangle Area: %.2f\n", area);
}

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork error \n");
        return 1;
    } 
    else if (pid == 0) {
        execl("./triangleArea", "triangleArea", NULL);
        perror("Exec failed\n");
        return 1;
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (status == 1) {
            perror("The Child process terminates with error\n");
        }
        rectangleArea();
    }

    return 0;
}