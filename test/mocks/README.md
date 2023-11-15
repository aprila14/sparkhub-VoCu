Actions performed for each of the headers to get a mock:
* Add a header path to `HEADERS_TO_COPY` list in `recreate_all_mocks.sh` script 
  Maintain the directory hierarchy to be able to identify the source.
* Add the file to be mocked to the list `MOCKS_TO_PROCESS` of files in the `recreate_all_mocks.sh` script.
* Run the script `recreate_all_mocks.sh` to copy headers and generate the mock
* Update the test project CMAKE to include the mocked file (see `MOCK_FILES` section)
* Compile the project.

*NOTES*
If the project does not compile some additional tampering with the headers need to be done.
* If there is a complaint about _ __STATIC_INLINE_ macro being undefined
edit the relevant header and remove it.
* If the function to be modified is doing some manipulation with MCU registers and is implemented
in the header - mock it. To do it remove the implementation with declaration.
