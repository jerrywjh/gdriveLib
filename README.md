gdriveLib
=========

A C++ implementation for Google Drive SDK

This implementation is coded based on [Google Drive SDK Document](https://developers.google.com/drive/). 

There are already [multiple libs for Google Drive in different programming languages](https://developers.google.com/drive/web/downloads), including Java, Javascript, .NET, Objective-C, PHP, Python, Go, Node.js...  However, C++ implementation lib is never availalbe from Google.

When I worked with this [Google Workspace backup tool](http://www.cubebackup.com),   some C++ implementation code was made, acting as a lib to support the main program to access Google Drive (as well as other Google Apps services). 

Note: This is not a fully implementation of Google Drive SDK, some features of GDrive manipulations are missing in this implementation. I hope opensourcing this project can help to make it better and more feature-riched.

[Libcurl](http://curl.haxx.se/libcurl/) is employed in this gdriveLib implementation. It is an incredible lib which I have been using for years in both C++ and PHP code, and it never fails me!

The original code was developed in Visual Studio, but I plan to drop all the platform-dependent stuff out and make the code workable on all Operating systems.



