# QVocabulary
A QT alternative of Microsoft Vocabulary++

For building use this vagrant machine: https://github.com/iatanasov77/my-desktop-projects

I. Howto Build Application
	1. First clone or download source in the separate folder
	2. Run `qmake` to create a Makefile
	3. Run `make`
	
II. Howto Build Help
	1. Enter into the Help folder
	2. Run `qcollectiongenerator qvocabulary.qhcp -o qvocabulary.qhc` OR `qhelpgenerator qvocabulary.qhcp -o qvocabulary.qhc`
	3. During deployment Create Help folder with qvocabulary.qch and qvocabulary.qhc files into the deployment package
	
