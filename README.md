# Real time caption generator using Microsoft Azure speech services (SubGen)
## Overview
This project is aim to provide real time captioning for any content with the help of speech transcription and translation technology. SubGen is capable to peform real time captioning for every application that runs on windows.
### How it works?
1. The audio output of the system is being streamed to the Microsoft Azure speech service.
2. The transcription result from the speech services will then being displayed in the subtitle window.
### Supported Platform
Windows: Tested in Windows 10 and Windows 11
### Technical Choice
1. Language: C++
2. UI Framework: [Qt](https://www.qt.io/product/framework)
3. Dependency: [rtaudio](https://github.com/thestk/rtaudio), [Azure Speech SDK](https://learn.microsoft.com/en-us/azure/cognitive-services/Speech-Service/speech-sdk)
4. External Driver: [VB-Cable](https://vb-audio.com/Cable/)

## Setup Guide
1. Install [VB-Cable](https://vb-audio.com/Cable/).
2. Download subgen.zip and extract it.
3. Run the subgen.exe from the file.
4. Add your own key and region for Azure speech service. 
### About Microsoft Azure Speech Service
You will required a Microsoft Azure account to use the speech service. You can register a free account at [Microsoft](https://azure.microsoft.com/en-us/free/) and create the speech resource to get the key and region information.
```diff
-Noted that the captioning will not work without the Azure Speech Service
```

## Screenshots
homepage
> <img src="https://user-images.githubusercontent.com/69668411/223078477-5b769337-7194-4f7b-a896-395ada70c87b.png" width="400">
setting
> <img src="https://user-images.githubusercontent.com/69668411/223094232-13600e20-b65e-40c3-9170-56eacf2e6489.png" width="300">
subtitle window
> <img src="https://user-images.githubusercontent.com/69668411/223098285-e527b56c-cd9e-47fe-93ca-9e4220907279.png">

