import os
import setuptools

os.environ['LDFLAGS'] = '-framework IOKit -framework Foundation'

with open("README.md", "r") as file:
    long_description = file.read()

setuptools.setup(
    name = "mactemperatures",
    version = "0.0.1",
    author="Joshua Unrau",
    author_email="contact@joshuaunrau.com",
    description="Python Package for Obtaining Temperature Sensor Readings on M1 Macs",
    long_description=long_description,
    long_description_content_type="text/markdown",
    url="https://github.com/joshunrau/MacTemperatures",
    project_urls={
        "Bug Tracker": "https://github.com/joshunrau/MacTemperatures/issues",
    },
    classifiers=[
        "Programming Language :: C",
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: MacOS",
    ],
    packages=setuptools.find_packages(where="."),
    python_requires=">=3.7",
    ext_modules = [
        setuptools.Extension(
            name="macsensors", 
            sources=[
                os.path.join('mactemperatures', 'sensors.c')
            ]
        )
    ]
)