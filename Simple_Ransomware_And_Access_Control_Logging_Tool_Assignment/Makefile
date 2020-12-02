all: logger acmonitor test_aclog test_ransomware

logger: logger.c
	gcc -Wall -fPIC -shared -o logger.so logger.c -lcrypto -ldl 
	#sudo chown root logger.so
	#sudo chmod 4577 logger.so

acmonitor: acmonitor.c 
	gcc -g acmonitor.c -o acmonitor
	#sudo chown root acmonitor
	#sudo chmod 4577 acmonitor

test_aclog: test_aclog.c 
	gcc -g test_aclog.c -o test_aclog
	#sudo chown root test_aclog
	#sudo chmod 4577 test_aclog

test_ransomware: test_ransomware.c
	gcc -g test_ransomware.c -o test_ransomware

run: logger.so test_aclog
	LD_PRELOAD=./logger.so ./test_aclog

clean:
	rm -rf logger.so
	rm -rf test_aclog
	rm -rf acmonitor
	rm -rf test_ransomware

