#include <gtest/gtest.h>
#include <thread>
#include <signal.h>
#include "process.h"
#include "signal_handler.h"


class App : public Gap::Process
{
  public:
    void Init();
  private:
    Gap::EventProvider m_eventProvider;
    Gap::SignalHandler m_signalHandler {{SIGINT, SIGQUIT}};

    void onSignal(uint32_t sigValue);
};

App app;
uint32_t receivedSignal = 0;

void App::Init()
{ 
  SetEventProvider(&m_eventProvider);  
  m_signalHandler.SetOnSignal([this](uint32_t signalId){onSignal(signalId);});
  m_eventProvider.AddConsumer(&m_signalHandler);
  
}
void App::onSignal(uint32_t sigValue)
{
  receivedSignal = sigValue;
}

static void Terminate()
{  
  usleep(250000);
  
  app.Terminate();
}

// Test receiving termination signal (SIGQUIT)
TEST(Process, SignalHandler) {

  std::thread termThread(Terminate);

  app.Init();  
  app.Run();
  
  EXPECT_EQ(receivedSignal, SIGQUIT);
  termThread.join();
  
}
