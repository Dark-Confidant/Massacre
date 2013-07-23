STYL_PATH := node_modules/stylus/bin/stylus

style.css: style.styl $(STYL_PATH)
	$(STYL_PATH) style.styl;

$(STYL_PATH):
	npm install stylus --save-dev
