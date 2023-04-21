

wallet_test_path := ./wallet_test/

own_path := ./

root_path := ./../

output_name := $(wallet_test_path)output

all:

	cp $(own_path)*.cpp $(root_path)
	cp $(own_path)*.h $(root_path)

test:

	cp $(wallet_test_path)*.cp $(root_path)
	cp $(own_path)*.cpp $(root_path)
	cp $(own_path)*.h $(root_path)

	g++ -o $(output_name) $(root_path)*.cpp $(root_path)*.c $(root_path)*.cp -g
	