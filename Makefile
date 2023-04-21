
crypto_path := ./crypto/
wallet_path := ./wallet/
utils_path := ./utils/
p2p_path := ./p2p/

wallet_test:

	$(MAKE) -C $(crypto_path)
	$(MAKE) -C $(utils_path)
	$(MAKE) -C $(p2p_path)

	$(MAKE) -C $(wallet_path) test

	$(MAKE) clean

p2p_test:

	$(MAKE) -C $(crypto_path)
	$(MAKE) -C $(utils_path)
	$(MAKE) -C $(wallet_path) 

	$(MAKE) -C $(p2p_path) test 

	$(MAKE) clean

clean:

	rm -f ./*.cpp
	rm -f ./*.c
	rm -f ./*.h
	rm -f ./*.o
	rm -f ./*.cp
	

