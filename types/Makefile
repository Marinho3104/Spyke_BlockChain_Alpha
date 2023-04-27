

block_path := ./block/

transaction_path := ./transaction/

types_test_path := ./types_test/

output_name := $(types_test_path)output

root_path := ./../

all:

	cp $(block_path)*.cpp $(transaction_path)*.cpp $(root_path)
	cp $(block_path)*.h $(transaction_path)*.h $(root_path)


test:

	cp $(types_test_path)*.cp $(root_path)
	cp $(block_path)*.cpp $(transaction_path)*.cpp $(root_path)
	cp $(block_path)*.h $(transaction_path)*.h $(root_path)

	g++ -o $(output_name) $(root_path)*.cpp $(root_path)*.c $(root_path)*.cp -g
	
