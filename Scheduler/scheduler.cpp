#include <iostream>
#include <queue>
#include <vector>

struct process {
    size_t burst_time;
    int remaining_time;
    pid_t pid;

    process(pid_t id, size_t bt) : pid(id), burst_time(bt), remaining_time(bt) {}
};

class Scheduler {
public:
    explicit Scheduler(size_t quantumTime) : _quantumTime(quantumTime) {}

    void startScheduling() {
        while (!_processQueue.empty()) {
            process current = _processQueue.front();
            _processQueue.pop();

            std::cout << "\n-> Executing Process PID: " << current.pid << "\n";
            std::cout << "Remaining Time: " << current.remaining_time << "\n";

            if (current.remaining_time > _quantumTime) {
                current.remaining_time -= _quantumTime;
                std::cout << "Not finished, re-queued with " << current.remaining_time << " remaining\n";
                _processQueue.push(current);
            } else {
                std::cout << "Process PID: " << current.pid << " finished.\n";
            }
        }
    }

    void addProcess(const process& p) {
        _processQueue.push(p);
    }

    void addProcesses(const std::vector<process>& processes) {
        for (const auto& p : processes) {
            _processQueue.push(p);
        }
    }

    void setQuantumTime(size_t quantumTime) {
        _quantumTime = quantumTime;
    }

    size_t getQuantumTime() const {
        return _quantumTime;
    }

private:
    size_t _quantumTime;
    std::queue<process> _processQueue;
};

int main() {
    size_t quantumTime = 4;
    Scheduler scheduler(quantumTime);

    scheduler.addProcesses({
        process(1, 10),
        process(2, 6),
        process(3, 2),
        process(4, 8)
    });

    scheduler.startScheduling();
    return 0;
}
