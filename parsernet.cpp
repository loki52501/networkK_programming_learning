#include<iostream>
#include<vector>
#include<stdexcept>
#include<cstdint>
using namespace std;
struct Message{
    uint8_t type;
    uint32_t length;
    vector<uint8_t> data;
};
Message parseMessage(vector<uint8_t> buffer)
{
    Message msg;
    if(buffer.size()<5)
    {
        throw runtime_error("Message is not good");
    }
    msg.type=buffer[0];
    msg.length=buffer[1]<<24|buffer[2]<<16|buffer[3]<<8|buffer[4];

    if(buffer.size()<5+msg.length)
        throw runtime_error("Incomplete Message");
    for(uint32_t i=0;i<msg.length;i++)
    msg.data.push_back(buffer[i+5]);
   for(uint32_t i=0;i<msg.length;i++)
       cout<<msg.data[i];
   return msg;
}
int main()
{
vector<uint8_t> testData = {
    0x01,                    // type = 1
    0x00, 0x00, 0x00, 0x03, // length = 3
    'A', 'B', 'C'           // data
};

Message msg = parseMessage(testData);
return 0;
}