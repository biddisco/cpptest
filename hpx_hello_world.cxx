#include <hpx/hpx.hpp>
#include <hpx/hpx_init.hpp>
#include <hpx/thread_executors/pool_executor.hpp>

#include <exception>
#include <functional>

int hpx_main(boost::program_options::variables_map &)
{
  hpx::threads::executors::pool_executor exec("default");
  auto f = [](hpx::future<int> &&p)
  {
      auto test = p.get() + 1;
      if (test != 6) {
          throw std::runtime_error("error in hpx futures");
      }
      std::cout << test << std::endl;
  };
  hpx::dataflow(exec, 
          std::move(f), 
          hpx::make_ready_future<int>(5)
    ).get();

  return hpx::finalize();
}

int main(int argc, char **argv)
{
    return hpx::init(hpx_main, argc, argv);
}


