#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
using namespace sf;

const float SCALE = 30.f;
const float DEG = 57.29577f;

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
    RenderWindow window(VideoMode(800,600), "Lao8 Volleyball");
    window.setFramerateLimit(60);

    Texture t_bg, t_bianpao, t_lao8;
    t_bg.loadFromFile("./pic/bg.png");
    t_bianpao.loadFromFile("./pic/bianpao.png");
    t_lao8.loadFromFile("./pic/lao8.png");

    Sprite sBackground(t_bg), sBianPao(t_bianpao), sLao8(t_lao8);
    sBianPao.setOrigin(32,32);

    //---box2d-------
    setWall(400,550,2000,10); //floor
    setWall(350,30,10,70);//upper obstacle
    setWall(350,400,10,120);//lower obstacle
    setWall(0,0,10,2000); //left wall
    setWall(800,0,10,2000); //right wall
    setWall(400,0,2000,10);//ceiling


    b2BodyDef bdef;
    bdef.type = b2_dynamicBody;
    bdef.position.Set(10,1);
    b2Body *b = World.CreateBody(&bdef);

    b2CircleShape circle;
    circle.m_radius = 32/SCALE;
    b2FixtureDef fdef;
    fdef.shape = &circle;
    fdef.restitution = 0.95;
    fdef.density = 0.2;
    b->CreateFixture(&fdef);
    int data = 1;
    b->SetUserData((void*)data);
    ////////players
    bdef.position.Set(20,2);
    b2CircleShape c;
    c.m_radius = 32/SCALE;
    c.m_p.Set(0,13/SCALE);
    b2Body *pBody = World.CreateBody(&bdef);
    pBody->CreateFixture(&c,5);
    c.m_radius = 25/SCALE;
    c.m_p.Set(0,-20/SCALE);
    pBody->CreateFixture(&c,5);
    pBody->SetFixedRotation(true);
    int data_2 = 2;
    pBody->SetUserData((void*)data_2);

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

        b2Vec2 pos = pBody->GetPosition();
        b2Vec2 vel = pBody->GetLinearVelocity();

        if(Keyboard::isKeyPressed(Keyboard::Right)) vel.x = 5;
        if(Keyboard::isKeyPressed(Keyboard::Left)) vel.x = -5;
        if(Keyboard::isKeyPressed(Keyboard::Up)) vel.y = -5;
        if(Keyboard::isKeyPressed(Keyboard::Down)) vel.y = 5;
        
        
        //if(Keyboard::isKeyPressed(Keyboard::Up)) if(pos.y*SCALE>463) vel.y = -13;

        if(!Keyboard::isKeyPressed(Keyboard::Right))
        if(!Keyboard::isKeyPressed(Keyboard::Left))
            vel.x = 0;

        if(!Keyboard::isKeyPressed(Keyboard::Up))
        if(!Keyboard::isKeyPressed(Keyboard::Down))
            vel.y = 0;

        pBody->SetLinearVelocity(vel);

        ///////Draw/////////
        window.draw(sBackground);

        for(b2Body* it = World.GetBodyList(); it!=0; it = it->GetNext())
        {
            b2Vec2 pos = it->GetPosition();
            float angle = it->GetAngle();

            if((uintptr_t)it->GetUserData()==2)
            {
                sLao8.setPosition(pos.x*SCALE, pos.y*SCALE);
                sLao8.setRotation(angle*DEG);
                window.draw(sLao8);
            }

            if((uintptr_t)it->GetUserData()==1)
            {
                sBianPao.setPosition(pos.x*SCALE, pos.y*SCALE);
                sBianPao.setRotation(angle*DEG);
                window.draw(sBianPao);
            }
        }
        window.display();
    }




    return 0;
}
