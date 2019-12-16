#include "catch.hpp"
#include "../include/Group.h"

TEST_CASE("test group initialization", "[group]") {
    Group *group = new Group(1000);
    REQUIRE(group->getGroupBudget() == 1000);
    delete group;
}

TEST_CASE("test group removing from budget", "[group]") {
    Group *group = new Group(1000);
    group->removeFromGroupRessources(100);
    REQUIRE(group->getGroupBudget() == 900);
    delete group;
}
