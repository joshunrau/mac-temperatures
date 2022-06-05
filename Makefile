.DEFAULT_GOAL:= clean-install

clean:
	rm -fr build/
	rm -fr dist/
	find . -name '*.egg-info' -exec rm -fr {} +
	find . -name '__pycache__' -exec rm -fr {} +

test:
	python -m unittest discover tests
	
install:
	pip install .

clean-install: install clean