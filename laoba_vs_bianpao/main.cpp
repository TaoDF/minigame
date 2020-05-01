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

    Texture t1, t2, t3;
    t1.loadFromFile("./pic/cesuo.png");
    t2.loadFromFile("./pic/bianpao.png");
    t3.loadFromFile("./pic/lao8.png");

    Sprite sBackground(t1), sBall(t2), sPlayer(t3);
    sBall.setOrigin(32,32);

    //---box2d-------
    setWall(400,520,2000,10);
    setWall(400,450,10,170);
    setWall(0,0,10,2000);
    setWall(800,0,10,2000);


   // b2PolygonShape shape;
   // shape.SetAsBox(30/SCALE,30/SCALE);
    b2BodyDef bdef;
    bdef.type = b2_dynamicBody;
    bdef.position.Set(5,1);
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
        if(Keyboard::isKeyPressed(Keyboard::Up)) if(pos.y*SCALE>463) vel.y = -13;

        if(!Keyboard::isKeyPressed(Keyboard::Right))
        if(!Keyboard::isKeyPressed(Keyboard::Left))
            vel.x = 0;

        pBody->SetLinearVelocity(vel);


        ///////Draw/////////
        window.draw(sBackground);

        for(b2Body* it = World.GetBodyList(); it!=0; it = it->GetNext())
        {
            b2Vec2 pos = it->GetPosition();
            float angle = it->GetAngle();

            if((uintptr_t)it->GetUserData()==2)
            {
                sPlayer.setPosition(pos.x*SCALE, pos.y*SCALE);
                sPlayer.setRotation(angle*DEG);
                window.draw(sPlayer);
            }

            if((uintptr_t)it->GetUserData()==1)
            {
                sBall.setPosition(pos.x*SCALE, pos.y*SCALE);
                sBall.setRotation(angle*DEG);
                window.draw(sBall);
            }

        }
        window.display();
    }




    return 0;
}
