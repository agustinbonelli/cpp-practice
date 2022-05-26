#include <memory>
#include <atomic>
#include <spdlog/spdlog.h>

namespace agb
{

template<typename T>
class shared_ptr
{
public:
    shared_ptr(T* p) : ctrl(new control_block()),ptr(p) {
        ctrl->count++;
        spdlog::info("Taking ownership of ptr = {}",static_cast<void*>(ptr));
        spdlog::info("count = {}",ctrl->count.load());
    }

    shared_ptr(const shared_ptr<T>& rhs)
    {

        ptr = rhs.ptr;
        ctrl = rhs.ctrl;
        int prev = ctrl->count.fetch_add(1);
        spdlog::info("Copy ctor. count before add = {}",prev);

    }

    shared_ptr(shared_ptr<T>&& rhs)
    {
        ptr = rhs.ptr;
        ctrl = rhs.ctrl;
        rhs.ptr=nullptr;
        rhs.ctrl=nullptr;
        spdlog::info("Move ctor. count  = {}",ctrl->count.load());
        // Don't increment count. As rhs stops owning ptr but this takes over.

    }

    ~shared_ptr()
    {
        if(ctrl && ptr)
        {
            spdlog::info("Destructor. count before substract= {}",ctrl->count.load());

            int count = --(ctrl->count);
            spdlog::info("count after substract= {}",ctrl->count.load());
            spdlog::info("count = {}",count);
            if(count < 1)
            {
                spdlog::info("Deleting ptr = {}",static_cast<void*>(ptr));
                delete ptr;
                delete ctrl;
            }
        }

    }
    T* get(){return ptr;}
    T operator*(){return *ptr;}
    int count() const
    {
        return ctrl->count.load();
    }
private:
    struct control_block
    {
        std::atomic<int> count{0};
    };
    control_block* ctrl;
    T* ptr;
};











}//namespace agb