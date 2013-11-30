#include <qpdf/QUtil.hh>
#include <qpdf/InsecureRandomDataProvider.hh>
#include <qpdf/SecureRandomDataProvider.hh>
#include <iostream>

class BogusRandomDataProvider: public RandomDataProvider
{
  public:
    virtual ~BogusRandomDataProvider()
    {
    }
    BogusRandomDataProvider()
    {
    }
    virtual void provideRandomData(unsigned char* data, size_t len)
    {
        for (size_t i = 0; i < len; ++i)
        {
            data[i] = static_cast<unsigned char>(i & 0xff);
        }
    }
};

int main()
{
    long r1 = QUtil::random();
    long r2 = QUtil::random();
    if (r1 == r2)
    {
        std::cout << "fail: two randoms were the same\n";
    }
    InsecureRandomDataProvider irdp;
    irdp.provideRandomData(reinterpret_cast<unsigned char*>(&r1), 4);
    irdp.provideRandomData(reinterpret_cast<unsigned char*>(&r2), 4);
    if (r1 == r2)
    {
        std::cout << "fail: two insecure randoms were the same\n";
    }
    SecureRandomDataProvider srdp;
    srdp.provideRandomData(reinterpret_cast<unsigned char*>(&r1), 4);
    srdp.provideRandomData(reinterpret_cast<unsigned char*>(&r2), 4);
    if (r1 == r2)
    {
        std::cout << "fail: two secure randoms were the same\n";
    }
    BogusRandomDataProvider brdp;
    QUtil::setRandomDataProvider(&brdp);
    r1 = QUtil::random();
    r2 = QUtil::random();
    if (r1 != r2)
    {
        std::cout << "fail: two bogus randoms were different\n";
    }
    unsigned char buf[4];
    QUtil::initializeWithRandomBytes(buf, 4);
    if (! ((buf[0] == 0) &&
           (buf[1] == 1) &&
           (buf[2] == 2) &&
           (buf[3] == 3)))
    {
        std::cout << "fail: bogus random didn't provide correct bytes\n";
    }
    std::cout << "random: end of tests\n";
    return 0;
}
