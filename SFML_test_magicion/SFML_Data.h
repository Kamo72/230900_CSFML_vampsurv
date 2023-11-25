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
                // Key�� ã�� ���� ��� �⺻���� ��ȯ�ϰų� ���� ó���� ������ �� �ֽ��ϴ�.
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

   
    
    
    RenderWindow * nowWindow; //������ �Ǵ� ������
    Dictionary<string, Texture*> textureDictionary;  //�ؽ��� �����

    //Draw�Լ��� �Ű����� ���Է½� ��ü�Ǵ� ����
    Color color = Color(Color::White.r, Color::White.g, Color::White.b);
    Color* drawColor = &color;
    Uint8 drawAlpha = 0;

}
