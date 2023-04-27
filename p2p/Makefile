
node_path := ./node/
protocols_path := ./protocols/
socket_path := ./socket/
protocols_path := ./protocols/

p2p_test_path := ./p2p_tests/

own_path := ./

root_path := ./../

output_name := $(p2p_test_path)output

first_dir_path := $(p2p_test_path)f
second_dir_path := $(p2p_test_path)s

all:

	cp $(node_path)*.cpp $(protocols_path)*.cpp $(socket_path)*.cpp $(protocols_path)*.cpp $(protocols_path)*.cu $(root_path)
	cp $(node_path)*.h $(protocols_path)*.h $(socket_path)*.h $(protocols_path)*.h $(root_path)

test:

	cp $(p2p_test_path)*.cp $(root_path)
	cp $(node_path)*.cpp $(protocols_path)*.cpp $(socket_path)*.cpp $(protocols_path)*.cpp $(protocols_path)*.cu $(root_path)
	cp $(node_path)*.h $(protocols_path)*.h $(socket_path)*.h $(protocols_path)*.h $(root_path)

	g++ $(root_path)*.cpp $(root_path)*.c $(root_path)*.cp -g -c
	nvcc -c $(protocols_path)*.cu -g
	nvcc $(own_path)*.o -o $(output_name)

	cp $(output_name) $(first_dir_path)
	cp $(output_name) $(second_dir_path)

	$(MAKE) clean
	
clean:

	rm -f *.o