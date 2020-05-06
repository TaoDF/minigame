#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <box2d/box2d.h>
#include <iostream>
#include <unistd.h>
#include <time.h>
using namespace sf;

const float SCALE = 30.f;
const float DEG = 57.29577f;

enum whatever{START, BIANPAO, LAOBA, KUCHA, BABA, END};

b2Vec2 Gravity(0.f, 9.8f);
b2World World(Gravity);

void setWall(int x, int y, int w, int h)
{
    b2PolygonShape gr;
    gr.SetAsBox(w/SCALE,h/SCALE);

    b2BodyDef bdef;
    bdef.position.Set(x/SCALE, y/SCALE);

    b2Body *b_ground = World.CreateBody(&bdef);
    b_ground->CreateFixture(&gr, 1);
}

int main()
{
    Sound puke_sound;
    SoundBuffer puke_buffer;
    if (!puke_buffer.loadFromFile("./sound/puke.wav"))
        std::cout<<"err"<<std::endl;
    puke_sound.setBuffer(puke_buffer);

    Sound aoligei_sound;
    SoundBuffer aoligei_buffer;
    if (!aoligei_buffer.loadFromFile("./sound/aoligei.wav"))
        std::cout<<"err"<<std::endl;
    aoligei_sound.setBuffer(aoligei_buffer);

    bool boom_flag = false;
    bool chibao_flag = false;

    RenderWindow window(VideoMode(800,600), "Lao8 Volleyball");
    window.setFramerateLimit(60);

    Texture t_bg, t_bianpao, t_lao8, t_kucha, t_boom, t_teng, t_baba, t_chibao, t_burger;
    t_bg.loadFromFile("./pic/bg.png");
    t_bianpao.loadFromFile("./pic/bianpao.png");
    t_lao8.loadFromFile("./pic/lao8.png");
    t_kucha.loadFromFile("./pic/kucha.png");
    t_boom.loadFromFile("./pic/boom.png");
    t_teng.loadFromFile("./pic/teng.png");
    t_baba.loadFromFile("./pic/baba.png");
    t_chibao.loadFromFile("./pic/chibao.png");
    t_burger.loadFromFile("./pic/burger.png");


    Sprite sBackground(t_bg), sBianPao(t_bianpao), sLao8(t_lao8), sKucha(t_kucha),sBoom(t_boom),sTeng(t_teng),sBaba(t_baba),sChibao(t_chibao),sBurger(t_burger);
    sBianPao.setOrigin(32,32);
    sLao8.setOrigin(32,32);
    sKucha.setOrigin(32,32);
    sBaba.setOrigin(32,32);

    setWall(400,550,2000,10); //floor
    setWall(350,30,10,70);//upper obstacle
    setWall(350,400,10,120);//lower obstacle
    setWall(0,0,10,2000); //left wall
    setWall(800,0,10,2000); //right wall
    setWall(400,0,2000,10);//ceiling

    //public body def//////////////
    b2BodyDef bdef;
    bdef.type = b2_dynamicBody;


    //Projectiles///////////////
    bdef.position.Set(3,1);
    b2Body *pB_bianpao = World.CreateBody(&bdef);
    b2CircleShape c_bianpao;
    c_bianpao.m_radius = 32/SCALE;
    b2FixtureDef fdef_bianpao;
    fdef_bianpao.shape = &c_bianpao;
    fdef_bianpao.restitution = 0.95;
    fdef_bianpao.density = 0.2;
    pB_bianpao->CreateFixture(&fdef_bianpao);
    int data_bianpao = BIANPAO;
    pB_bianpao->SetUserData((void*)data_bianpao);

    bdef.position.Set(17,1);
    b2Body *pB_baba = World.CreateBody(&bdef);
    // b2CircleShape c_baba;
    // c_baba.m_radius = 32/SCALE;
    // b2FixtureDef fdef_baba;
    // fdef_baba.shape = &c_baba;
    // fdef_baba.restitution = 0.95;
    // fdef_baba.density = 0.2;
    pB_baba->CreateFixture(&fdef_bianpao);
    int data_baba = BABA;
    pB_baba->SetUserData((void*)data_baba);

    ////////players
    bdef.position.Set(2,15);
    b2CircleShape c_lao8;
    c_lao8.m_radius = 25/SCALE;
    c_lao8.m_p.Set(0,-20/SCALE);
    b2Body *pB_laoba = World.CreateBody(&bdef);
    pB_laoba->CreateFixture(&c_lao8,5);
    pB_laoba->SetFixedRotation(true);
    int data_laoba = LAOBA;
    pB_laoba->SetUserData((void*)data_laoba);


    bdef.position.Set(18,15);
    // b2CircleShape c_kucha;
    // c_kucha.m_radius = 25/SCALE;
    // c_kucha.m_p.Set(0,-20/SCALE);
    b2Body *pB_kucha = World.CreateBody(&bdef);
    pB_kucha->CreateFixture(&c_lao8,5);
    pB_kucha->SetFixedRotation(true);
    int data_kucha = KUCHA;
    pB_kucha->SetUserData((void*)data_kucha);

    while(window.isOpen())
    {
        Event e;
        while(window.pollEvent(e))
        {
            if(e.type==Event::Closed)
            {
                window.close();
            }
        }

        World.Step(1/60.f, 8, 3);

        b2Vec2 pos_laoba = pB_laoba->GetPosition();
        b2Vec2 vel_laoba = pB_laoba->GetLinearVelocity();

        b2Vec2 pos_kucha = pB_kucha->GetPosition();
        b2Vec2 vel_kucha = pB_kucha->GetLinearVelocity();


        if(Keyboard::isKeyPressed(Keyboard::Right)) vel_laoba.x = 5;
        if(Keyboard::isKeyPressed(Keyboard::Left)) vel_laoba.x = -5;
        if(Keyboard::isKeyPressed(Keyboard::Up)) if(pos_laoba.y*SCALE>463) vel_laoba.y = -10;

        if(!Keyboard::isKeyPressed(Keyboard::Right))
        if(!Keyboard::isKeyPressed(Keyboard::Left))
            vel_laoba.x = 0;

        pB_laoba->SetLinearVelocity(vel_laoba);


        if(Keyboard::isKeyPressed(Keyboard::D)) vel_kucha.x = 5;
        if(Keyboard::isKeyPressed(Keyboard::A)) vel_kucha.x = -5;
        if(Keyboard::isKeyPressed(Keyboard::W)) if(pos_kucha.y*SCALE>463) vel_kucha.y = -10;

        if(!Keyboard::isKeyPressed(Keyboard::D))
        if(!Keyboard::isKeyPressed(Keyboard::A))
            vel_kucha.x = 0;

        pB_kucha->SetLinearVelocity(vel_kucha);        

        ///////Draw/////////
        window.draw(sBackground);

        for(b2Body* it = World.GetBodyList(); it!=0; it = it->GetNext())
        {
            b2Vec2 pos = it->GetPosition();
            float angle = it->GetAngle();

            if((uintptr_t)it->GetUserData()==LAOBA)
            {
                sLao8.setPosition(pos.x*SCALE, pos.y*SCALE);
                sLao8.setRotation(angle*DEG);
                window.draw(sLao8);
            }

            if((uintptr_t)it->GetUserData()==KUCHA)
            {
                sKucha.setPosition(pos.x*SCALE, pos.y*SCALE);
                sKucha.setRotation(angle*DEG);
                window.draw(sKucha);
            }            

            if((uintptr_t)it->GetUserData()==BIANPAO)
            {
                if((pos.x*SCALE > pos_kucha.x*SCALE-75 && pos.x*SCALE < pos_kucha.x*SCALE+75 && 
                pos.y*SCALE < pos_kucha.y*SCALE+75 && pos.y*SCALE > pos_kucha.y*SCALE-75) || boom_flag)
                {
                    boom_flag = true;
                    sBoom.setPosition(pos_kucha.x*SCALE, pos_kucha.y*SCALE);
                    window.draw(sBoom);
                    sTeng.setPosition(0, 30);
                    window.draw(sTeng);
                    aoligei_sound.play();
                    window.display();
                    sleep(20);
                }
                else
                {
                    sBianPao.setPosition(pos.x*SCALE, pos.y*SCALE);
                    sBianPao.setRotation(angle*DEG);
                    window.draw(sBianPao);
                }
            }


            if((uintptr_t)it->GetUserData()==BABA)
            {
                if((pos.x*SCALE > pos_laoba.x*SCALE-75 && pos.x*SCALE < pos_laoba.x*SCALE+75 && 
                pos.y*SCALE < pos_laoba.y*SCALE+75 && pos.y*SCALE > pos_laoba.y*SCALE-75) || chibao_flag)
                {
                    chibao_flag = true;
                    sBurger.setPosition(pos_laoba.x*SCALE, pos_laoba.y*SCALE);
                    window.draw(sBurger);
                    sChibao.setPosition(0, 30);
                    window.draw(sChibao);
                    puke_sound.play();
                    window.display();
                    sleep(20);
                }
                else
                {
                    sBaba.setPosition(pos.x*SCALE, pos.y*SCALE);
                    sBaba.setRotation(angle*DEG);
                    window.draw(sBaba);
                }
            }

        }
        window.display();
    }

    return 0;
}
