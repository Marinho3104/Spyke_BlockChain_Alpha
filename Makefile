
block_chain_path := ./block_chain/
crypto_path := ./crypto/
wallet_path := ./wallet/
types_path := ./types/
utils_path := ./utils/
p2p_path := ./p2p/

p2p_test:

	$(MAKE) -C $(block_chain_path)
	$(MAKE) -C $(crypto_path)
	$(MAKE) -C $(utils_path)
	$(MAKE) -C $(wallet_path) 
	$(MAKE) -C $(types_path) 

	$(MAKE) -C $(p2p_path) test 

	$(MAKE) clean

types_test:

	$(MAKE) -C $(block_chain_path)
	$(MAKE) -C $(crypto_path)
	$(MAKE) -C $(utils_path)
	$(MAKE) -C $(wallet_path) 
	$(MAKE) -C $(p2p_path)

	$(MAKE) -C $(types_path) test

	$(MAKE) clean
		
wallet_test:

	$(MAKE) -C $(block_chain_path)
	$(MAKE) -C $(crypto_path)
	$(MAKE) -C $(utils_path)
	$(MAKE) -C $(types_path) 
	$(MAKE) -C $(p2p_path)

	$(MAKE) -C $(wallet_path) test

	$(MAKE) clean

clean:

	rm -f ./*.cpp
	rm -f ./*.c
	rm -f ./*.h
	rm -f ./*.o
	rm -f ./*.cp
	rm -f ./*.cu

