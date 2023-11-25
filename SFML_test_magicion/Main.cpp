#include "SFML_Custom.h"
#include "SFML_Data.h"
#include "Game.h"

using namespace std;
using namespace sf;

using namespace SFML_Custom;
using namespace SFML_Data;
using namespace Game;

int main()
{
    sf::VideoMode fullscreenMode = VideoMode::getDesktopMode(); // 원하는 해상도로 변경
    screenSize = Vector2i(fullscreenMode.width, fullscreenMode.height);
    RenderWindow window(fullscreenMode, "Kamo's SFML Project!", sf::Style::Fullscreen);

    nowWindow = &window;
    ProcessInitiate();  //프로그램의 전처리를 담당

    //Loop State
    while (nowWindow->isOpen())
    {
        //Initiate in Loop
        Event event;
        while (nowWindow->pollEvent(event))
        {
            if (event.type == Event::Closed) { nowWindow->close(); }
            im->TotalProcess(event);   //입력 관리자 프로세스
        }
        FrameTimeRefresh();

        //Do in Loop
        TotalSystem();      //프로그램의 루프처리를 담당
    }

    //End State
    return 0;
}