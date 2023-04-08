all:
	@echo "BUILDING BEATBOX"
	@echo ''
	cd as3; make all
	@echo ''
	@echo 'BUILDING NODE.JS SERVER'
	@echo ''
	cd as3-server; make all