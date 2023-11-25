#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <list>

using namespace std;
using namespace sf;

namespace SFML_Data {

    template <typename Key, typename Value>
    class Dictionary
    {
    private:
        map<Key, Value> data;

    public:
        void Add(const Key& key, const Value& value)
        {
            data[key] = value;
        }

        Value Get(const Key& key) const
        {
            auto it = data.find(key);
            if (it != data.end())
            {
                return it->second;
            }
            else
            {
                // Key를 찾지 못한 경우 기본값을 반환하거나 예외 처리를 수행할 수 있습니다.
                throw out_of_range("Key not found in the dictionary.");
            }
        }
    };

    template <typename Key, typename Value>
    class DictionaryForUniquePtr
    {
    private:
        map<Key, shared_ptr<Value>> data;

    public:
        void Add(const Key& key, const shared_ptr<Value>& value)
        {
            data[key] = value;
        }

        shared_ptr<Value> Get(const Key& key) const
        {
            auto it = data.find(key);
            if (it != data.end())
            {
                return it->second;
            }
            else
            {
                throw out_of_range("Key not found in the dictionary.");
            }
        }
    };

   
    
    
    RenderWindow * nowWindow; //기준이 되는 윈도우
    Dictionary<string, Texture*> textureDictionary;  //텍스쳐 저장소

    //Draw함수에 매개변수 미입력시 대체되는 값들
    Color color = Color(Color::White.r, Color::White.g, Color::White.b);
    Color* drawColor = &color;
    Uint8 drawAlpha = 0;

}
