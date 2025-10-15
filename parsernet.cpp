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
    {cout<<msg.length;
        throw runtime_error("Incomplete Message ");
    }
        for(uint32_t i=0;i<msg.length;i++)
    msg.data.push_back(buffer[i+5]);

   return msg;
}

vector<uint8_t> createMessage(uint8_t type, string data)
{
    vector<uint8_t> buffer;
    buffer.push_back(type);
    uint32_t length=data.length();
    uint8_t byte4=length>>24 & 0xff;
    uint8_t byte3=length>>16 & 0xff;
    uint8_t byte2=length>>8 & 0xff;
    uint8_t byte1=length & 0xff;
    buffer.push_back(byte4);
    buffer.push_back(byte3);
    buffer.push_back(byte2);
    buffer.push_back(byte1);
    
    for(char c: data)
    {
        buffer.push_back(c);
    }
    return buffer;
    

}

int main()
{
string bigData(1000000, 'X');
vector<uint8_t> testing=createMessage(5,bigData);

Message msg1 = parseMessage(testing);
for(auto s : msg1.data)
cout<<s;

return 0;
}