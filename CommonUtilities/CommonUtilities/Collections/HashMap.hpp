#pragma once
#include <cstdint>
#include <vector>
namespace CommonUtilities
{
#define HASH_SEED 234298734U
	inline unsigned int MurmurHash2(const void* key, int len, unsigned int seed)
	{
		// 'm' and 'r' are mixing constants generated offline.
		// They're not really 'magic', they just happen to work well.

		const unsigned int m = 0x5bd1e995;
		const int r = 24;

		// Initialize the hash to a 'random' value

		unsigned int h = seed ^ len;

		// Mix 4 bytes at a time into the hash

		const unsigned char* data = (const unsigned char*)key;

		while (len >= 4)
		{
			unsigned int k = *(unsigned int*)data;

			k *= m;
			k ^= k >> r;
			k *= m;

			h *= m;
			h ^= k;

			data += 4;
			len -= 4;
		}

		// Handle the last few bytes of the input array

		switch (len)
		{
		case 3: h ^= data[2] << 16;
		case 2: h ^= data[1] << 8;
		case 1: h ^= data[0];
			h *= m;
		};

		// Do a few final mixes of the hash to ensure the last few
		// bytes are well-incorporated.

		h ^= h >> 13;
		h *= m;
		h ^= h >> 15;

		return h;
	}
	template<class Key>
	inline uint32_t Hash(const Key& aKey)
	{

		return MurmurHash2(&aKey, sizeof(aKey), HASH_SEED);
	}

	template<>
	inline uint32_t Hash(const std::string& aString)
	{
		return MurmurHash2(reinterpret_cast<const void*>(aString.c_str()), static_cast<int>(aString.size()), HASH_SEED);
	}



	template <class Key, class Value>
	class HashMap
	{
	public:
		//Skapar en HashMap med plats för aCapacity element. Den behöver inte
		//kunna växa. Blir den full så blir den full.
		HashMap(int aCapacity);
		~HashMap();
		//Stoppar in aValue under nyckeln aKey. Om det fanns ett värde innan med
		//aKey som nyckel så skrivs det värdet över. Om det lyckas returneras
		//true. Om map:en är full så görs ingenting, och false returneras.
		bool Insert(const Key& aKey, const Value& aValue);
		//Plockar bort elementet med nyckeln aKey, om det finns. Om det finns
		//returneras true. Om det inte finns görs ingenting, och false
		//returneras.
		bool Remove(const Key& aKey);
		//Ger en pekare till värdet med nyckeln aKey, om det finns. Om det inte
		//finns returneras nullptr.
		const Value* Get(const Key& aKey) const;
		//Som ovan, men returnerar en icke-const-pekare.
		Value* Get(const Key& aKey);
	private:
		enum class State
		{
			Empty,
			InUse,
			Removed
		};
		struct Element
		{
			Value Value;
			State State = State::Empty;
			uint32_t Hash;
		};
		const int myCapacity;
		Element* myValues;
		int GetindexOfHash(uint32_t aHash) const;


	};

	template<class Key, class Value>
	inline HashMap<Key, Value>::HashMap(int aCapacity) : myCapacity(aCapacity)
	{
		myValues = new Element[myCapacity];
	}

	template<class Key, class Value>
	inline HashMap<Key, Value>::~HashMap()
	{
		delete[] myValues;
	}




	template<class Key, class Value>
	inline bool HashMap<Key, Value>::Insert(const Key& aKey, const Value& aValue)
	{
		uint32_t hash = Hash<Key>(aKey);
		if (myCapacity <= 0)
		{
			return false;
		}

		int index = GetindexOfHash(hash);
		if (index != -1)
		{
			myValues[index].Value = aValue;
			myValues[index].State = State::InUse;
			myValues[index].Hash = hash;
			return true;
		}

		return false;
	}

	template<class Key, class Value>
	inline bool HashMap<Key, Value>::Remove(const Key& aKey)
	{
		uint32_t hash = Hash<Key>(aKey);
		if (myCapacity <= 0)
		{
			return false;
		}
		int index = GetindexOfHash(hash);
		if (index != -1 && myValues[index].State == State::InUse)
		{
			myValues[index].State = State::Removed;
			return true;
		}
		return false;
	}

	template<class Key, class Value>
	inline const Value* HashMap<Key, Value>::Get(const Key& aKey) const
	{
		uint32_t hash = Hash<Key>(aKey);
		if (myCapacity <= 0)
		{
			return nullptr;
		}
		int index = GetindexOfHash(hash);
		if (index != -1 && myValues[index].State == State::InUse)
		{
			return &myValues[index].Value;
		}
		return nullptr;
	}

	template<class Key, class Value>
	inline Value* HashMap<Key, Value>::Get(const Key& aKey)
	{
		uint32_t hash = Hash<Key>(aKey);
		if (myCapacity <= 0)
		{
			return nullptr;
		}

		int index = GetindexOfHash(hash);
		if (index != -1 && myValues[index].State == State::InUse)
		{
			return &myValues[index].Value;
		}
		return nullptr;
	}

	template<class Key, class Value>
	inline int HashMap<Key, Value>::GetindexOfHash(uint32_t aHash) const
	{
		int index = aHash % myCapacity;
		for (int i = 0; i < myCapacity; i++)
		{
			if ((myValues[index].Hash == aHash && myValues[index].State == State::InUse) || myValues[index].State == State::Empty)
			{
				return index;
			}
			else
			{
				index += 1;
				if (index == myCapacity)
				{
					index = 0;
				}
			}
		}
		return -1;
	}



}