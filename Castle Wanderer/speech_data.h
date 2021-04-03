#pragma once
#include <vector>
#include <string>
////////////////////////////////////////////// SPEECH DATA //////////////////////////////////////////////////

///// Good
const std::vector<std::string> good_call = { "Hey guys! ","Oh wait! ","Just a little information: " };

const std::vector<std::string> good_s13 = { "I ","These little gobs ","The guys in shining amor over there " };
const std::vector<std::string> good_v13 = { "need ","just got ","can get you " };
const std::vector<std::string> good_o13 = { "some good stuff from the castle.","that staff you always wanted.","a horse." };

const std::vector<std::string> good_s2 = { "My little chad here ","Who you're looking for ","That goodlooking guy " };
const std::vector<std::string> good_v2 = { "will be met with ","wanted to be ","was once " };
const std::vector<std::string> good_o2 = { "a beautiful elf.","the king? ","your fellow!" };

///// Bad
const std::vector<std::string> notgood_call = { "Wow... ","Dude... ","Oh hey! " };

const std::vector<std::string> notgood_s13 = { "I ","Dude... ","Oh hey! " };
const std::vector<std::string> notgood_v13 = { "I ","Dude... ","Oh hey! " };
const std::vector<std::string> notgood_o13 = { "Hey guys, ","Dude... ","Oh hey! " };

const std::vector<std::string> notgood_s2 = { "Hey guys, ","Dude... ","Oh hey! " };
const std::vector<std::string> notgood_v2 = { "Hey guys, ","Dude... ","Oh hey! " };
const std::vector<std::string> notgood_o2 = { "Hey guys, ","Dude... ","Oh hey! " };

int speechCount = good_call.size();