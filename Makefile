FIRSTNAME=jade
LASTNAME=simien

ROOTNAME=$(LASTNAME)_$(FIRSTNAME)_HW
HW=3
FOPTION=_main
RUNOPTIONS="Prompt> "
CC=gcc
CFLAGS= -g -I.
LIBS =-l pthread
DEPS = 
OBJ = $(ROOTNAME)$(HW)$(FOPTION).o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) 

$(ROOTNAME)$(HW)$(FOPTION): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm *.o $(ROOTNAME)$(HW)$(FOPTION)

run: $(ROOTNAME)$(HW)$(FOPTION)
	./$(ROOTNAME)$(HW)$(FOPTION) $(RUNOPTIONS)
