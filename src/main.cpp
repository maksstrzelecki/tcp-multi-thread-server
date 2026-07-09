#include <iostream>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

class ThreadPool {
public:
    ThreadPool(size_t threads_num = std::thread::hardware_concurrency()) {
        for (size_t i = 0; i < threads_num; ++i) {
            threads_.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(mtx_);

                        cv_.wait(lock, [this] {
                            return !jobQueue_.empty() || stop_flag_;
                        });

                        if (jobQueue_.empty() && stop_flag_) return;

                        task = std::move(jobQueue_.front());
                        jobQueue_.pop();
                    }   
                    task();
                }
            });
        }
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(mtx_);
            stop_flag_ = true;
        }
        cv_.notify_all();

        for (auto& t : threads_) t.join();
    }

    void add_task(std::function<void()> task) {
        {
            std::unique_lock<std::mutex> lock(mtx_);
            jobQueue_.emplace(std::move(task));
        }
        cv_.notify_one();
    }

private:
    std::vector<std::thread> threads_;
    std::queue<std::function<void()>> jobQueue_;
    std::mutex mtx_;
    std::condition_variable cv_;
    bool stop_flag_ = false;
};


int main() {
    int server_fd;
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);


    if (bind(server_fd, (sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind");
        return 1;
    }

    if (listen(server_fd, 10) < 0) {
        perror("listen");
        return 1;
    }

    // creating a thread pool
    ThreadPool pool;

    std::cout << "Server listening on port 8080\n";


    while (true) {
        sockaddr_in client{};
        socklen_t client_size = sizeof(client);

        int client_fd;
        if ((client_fd = accept(server_fd, (sockaddr*)&client, &client_size)) < 0)
            perror("accept");
            break;

        pool.add_task([client_fd] {
            char buffer[1024];
            memset(buffer, 0, sizeof(buffer));

            std::cout << "Client\n";

            int recv_bytes;
            while ((recv_bytes = recv(client_fd, buffer, sizeof(buffer), 0)) > 0) {
                std::string message(buffer, recv_bytes);
                std::cout << message;
            }

            std::string response = "Hello from server\n";

            int sent_bytes_overall = 0;
            int sent_bytes;
            while (sent_bytes_overall < response.size()) {
                sent_bytes = send(
                    client_fd,
                    response.data() + sent_bytes_overall,
                    response.size() - sent_bytes_overall,
                    0
                );

                if (sent_bytes <= 0) break;
                sent_bytes_overall += sent_bytes;
            }
            close(client_fd);
        });
    }
    close(server_fd);
    return 0;
}