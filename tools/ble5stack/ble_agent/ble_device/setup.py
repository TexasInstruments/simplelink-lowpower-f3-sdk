import setuptools

with open("README.md", "r") as fh:
    long_description = fh.read()


setuptools.setup(
    name="ti-simplelink-ble-device",
    version="0.0.7",
    author="Texas Instruments",
    author_email="support@ti.com",
    description="Simple API for BLE device feature",
    long_description=long_description,
    long_description_content_type="text/markdown",
    url="",
    packages=setuptools.find_packages(),
    classifiers=[
        "Programming Language :: Python :: 3.12",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
    ],
    install_requires=[
        "ti-simplelink-unpi",
        "ti-simplelink-rtls"
    ],
)