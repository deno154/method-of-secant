#include "handlers.h"

#include "../algorithms/vigenere/vigenere.h"
#include "../algorithms/md5/md5.h"
#include "../algorithms/secant/secant.h"
#include "../algorithms/graph_cycle/graph_cycle.h"

std::string handleVigenere(const std::string &body)
{

    return vigenereStub(body);
}

std::string handleMD5(const std::string &body)
{

    return md5Stub(body);
}

std::string handleSecant(const std::string &body)
{

    return secantStub(body);
}

std::string handleGraphCycle(const std::string &body)
{

    return graphCycleStub(body);
}