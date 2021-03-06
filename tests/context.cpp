#include <catch.hpp>
#include <zmq.hpp>

#if (__cplusplus >= 201703L)
static_assert(std::is_nothrow_swappable<zmq::context_t>::value,
              "context_t should be nothrow swappable");
#endif

TEST_CASE("context construct default and destroy", "[context]")
{
    zmq::context_t context;
}

TEST_CASE("context create, close and destroy", "[context]")
{
    zmq::context_t context;
    context.close();
    CHECK(NULL == (void *) context);
}

TEST_CASE("context shutdown", "[context]")
{
    zmq::context_t context;
    context.shutdown();
    CHECK(NULL != (void *) context);
    context.close();
    CHECK(NULL == (void *) context);
}

TEST_CASE("context shutdown again", "[context]")
{
    zmq::context_t context;
    context.shutdown();
    context.shutdown();
    CHECK(NULL != (void *) context);
    context.close();
    CHECK(NULL == (void *) context);
}

#ifdef ZMQ_CPP11
TEST_CASE("context swap", "[context]")
{
    zmq::context_t context1;
    zmq::context_t context2;
    using std::swap;
    swap(context1, context2);
}

TEST_CASE("context - use socket after shutdown", "[context]")
{
    zmq::context_t context;
    zmq::socket_t sock(context, zmq::socket_type::rep);
    context.shutdown();
    try
    {
        sock.connect("inproc://test");
        zmq::message_t msg;
        sock.recv(msg, zmq::recv_flags::dontwait);
        REQUIRE(false);
    }
    catch (const zmq::error_t& e)
    {
        REQUIRE(e.num() == ETERM);
    }
}
#endif
