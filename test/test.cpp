#include <sys/timerfd.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>
#include <assert.h>
#include <iostream>
#include <functional>
#include <map>

using namespace std;

typedef std::function<void()> Functor;

static uint64_t now() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 * 1000 + tv.tv_usec;
};

class Timer {
public:
    Timer() {
        fd_ = timerfd_create(CLOCK_MONOTONIC, 0);
        assert(fd_ != 0);
        first_time_ =
            now() + (uint64_t)365 * 24 * 3600 * 1000000;
    };
    void Queue(uint64_t time, const Functor& f) {
        uint64_t time_tmp = time + now();
        callbacks_[time_tmp] = f;
        if (time < first_time_) {
            setnext(time);
        };
    };

    void run() {
        while (true) {
            uint64_t exp;
            int s = read(fd_, &exp, sizeof(uint64_t));
            if (s != sizeof(uint64_t)) {
                cout << "error occr in read" << endl;
                exit(-1);
            };
            
            auto i = callbacks_.begin();
            uint64_t time = now();
            while (i != callbacks_.end() && i->first <= time ) {
                i->second();
                i++;
            }
            callbacks_.erase(callbacks_.begin(), i);

            if (callbacks_.empty()) {
                break;
            } else {
                auto i = callbacks_.begin();
                uint64_t t = i->first - now();
                assert(t > 0);
                setnext(t);
            }
        }
    };
private:
    void setnext(uint64_t time) {
        struct itimerspec data;
        data.it_value.tv_sec = time / 1000000;
        data.it_value.tv_nsec = (time % 1000000) * 1000;

        data.it_interval.tv_sec = 0;
        data.it_interval.tv_nsec = 0;

        timerfd_settime(fd_, 0, &data, NULL);
        first_time_ = now() + time;
    };
    int fd_;

    uint64_t first_time_;
    // end time . in microsenonds
    std::map<uint64_t, Functor> callbacks_;

    Timer & operator=(const Timer&);
    Timer(const Timer&);
};

void test()
{
    //cout << "why ???" << endl;
    static struct timespec start;
    struct timespec curr;
    static int first_call = 1;
    int secs, nsecs;

    if (first_call) {
        first_call = 0;
        if (clock_gettime(CLOCK_MONOTONIC, &start) == -1){
            cout << ("clock_gettime") << endl;
        }
    }

    if (clock_gettime(CLOCK_MONOTONIC, &curr) == -1){
        cout << ("clock_gettime") << endl;
    }

    secs = curr.tv_sec - start.tv_sec;
    nsecs = curr.tv_nsec - start.tv_nsec;
    if (nsecs < 0) {
        secs--;
        nsecs += 1000000000;
    }
    printf("%d.%03d \n", secs, (nsecs ) / 1000000);
}

static Timer &  GetTimer(){
    static Timer t;
    return t;
};

void dosomething(){
    test();
    GetTimer().Queue(1000000,dosomething);
}

int main() {
    test();
    GetTimer().Queue(1000000,dosomething);
    GetTimer().run();
}

