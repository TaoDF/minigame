#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
using namespace sf;

const float SCALE = 30.f;
const float DEG = 57.29577f;

enum whatever{START, BIANPAO, LAOBA, END};

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
    bool boom_flag = false;

    RenderWindow window(VideoMode(800,600), "Lao8 Volleyball");
    window.setFramerateLimit(60);

    Texture t_bg, t_bianpao, t_lao8, t_kucha, t_boom, t_teng;
    t_bg.loadFromFile("./pic/bg.png");
    t_bianpao.loadFromFile("./pic/bianpao.png");
    t_lao8.loadFromFile("./pic/lao8.png");
    t_kucha.loadFromFile("./pic/kucha.png");
    t_boom.loadFromFile("./pic/boom.png");
    t_teng.loadFromFile("./pic/teng.png");

    Sprite sBackground(t_bg), sBianPao(t_bianpao), sLao8(t_lao8), sKucha(t_kucha),sBoom(t_boom),sTeng(t_teng);
    sBianPao.setOrigin(32,32);
    sLao8.setOrigin(32,32);
    sKucha.setOrigin(32,32);
    int kucha_x = 600;
    int kucha_y = 300;
    sKucha.setPosition(kucha_x,kucha_y);

    setWall(400,550,2000,10); //floor
    setWall(350,30,10,70);//upper obstacle
    setWall(350,400,10,120);//lower obstacle
    setWall(0,0,10,2000); //left wall
    setWall(800,0,10,2000); //right wall
    setWall(400,0,2000,10);//ceiling


    b2BodyDef bdef;
    bdef.type = b2_dynamicBody;
    bdef.position.Set(3,1);
    b2Body *pB_bianpao = World.CreateBody(&bdef);

    b2CircleShape circle;
    circle.m_radius = 32/SCALE;
    b2FixtureDef fdef;
    fdef.shape = &circle;
    fdef.restitution = 0.95;
    fdef.density = 0.2;
    pB_bianpao->CreateFixture(&fdef);
    int data_bianpao = BIANPAO;
    pB_bianpao->SetUserData((void*)data_bianpao);
    ////////players
    bdef.position.Set(2,15);
    b2CircleShape c;
    c.m_radius = 32/SCALE;
    c.m_p.Set(0,13/SCALE);
    b2Body *pB_laoba = World.CreateBody(&bdef);
    pB_laoba->CreateFixture(&c,5);
    c.m_radius = 25/SCALE;
    c.m_p.Set(0,-20/SCALE);
    pB_laoba->CreateFixture(&c,5);
    pB_laoba->SetFixedRotation(true);
    int data_laoba = LAOBA;
    pB_laoba->SetUserData((void*)data_laoba);

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

        b2Vec2 pos = pB_laoba->GetPosition();
        b2Vec2 vel = pB_laoba->GetLinearVelocity();

        if(Keyboard::isKeyPressed(Keyboard::Right)) vel.x = 5;
        if(Keyboard::isKeyPressed(Keyboard::Left)) vel.x = -5;
       
        if(Keyboard::isKeyPressed(Keyboard::Up)) if(pos.y*SCALE>463) vel.y = -10;

        if(!Keyboard::isKeyPressed(Keyboard::Right))
        if(!Keyboard::isKeyPressed(Keyboard::Left))
            vel.x = 0;

        pB_laoba->SetLinearVelocity(vel);

        ///////Draw/////////
        window.draw(sBackground);
        window.draw(sKucha);


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

            if((uintptr_t)it->GetUserData()==BIANPAO)
            {
                if((pos.x*SCALE > kucha_x-55 && pos.x*SCALE < kucha_x+55 && 
                pos.y*SCALE < kucha_y+55 && pos.y*SCALE > kucha_y-55) || boom_flag)
                {
                    boom_flag = true;
                    sBoom.setPosition(kucha_x, kucha_y);
                    window.draw(sBoom);
                    sTeng.setPosition(0, 30);
                    window.draw(sTeng);
                }
                else
                {
                    sBianPao.setPosition(pos.x*SCALE, pos.y*SCALE);
                    sBianPao.setRotation(angle*DEG);
                    window.draw(sBianPao);
                }
            }
        }
        window.display();
    }

    return 0;
}
