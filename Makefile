
NAME=cscrypto
OBJS=

.PHONY: clean

lib$(NAME).so.1.0: lib$(NAME).a
	g++ -shared -s -Wl,-soname,lib$(NAME).so.1 -o lib$(NAME).so.1.0 lib$(NAME).a

lib$(NAME).a: $(OBJS)
	ar -cq lib$(NAME).a $(OBJS)

clean:
	rm -rf *.o *.a *.so.*

