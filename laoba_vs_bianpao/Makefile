TARGET = laoba

CPP_FILES = 
BASE = $(basename $(CPP_FILES))
OBJS = $(addsuffix .o, $(BASE))
LIBS = -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system -lbox2d

$(TARGET):$(OBJS)

	-rm -f $@

	g++ -std=c++11 -o $(TARGET) $(LIBS) main.cpp $(OBJS) 



%.o:%.cpp


	g++ -c -o $@ $<


clean:

	-rm -f laoba

	-rm -f *.o
