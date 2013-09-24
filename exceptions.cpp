#include <iostream>

class HexAddressOutOfBoundsException : public std::exception {};

class HexMapUninitializedException : public std::exception {};

class FrameOutOfRangeException : public std::exception {};

class StateOutOfRangeException : public std::exception {};
