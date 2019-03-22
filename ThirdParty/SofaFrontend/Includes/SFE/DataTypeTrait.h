// Ce fichier est un composant de la bibliothèque SofaFrontEnd développée 
// par Digital-Trainers SAS. 
//
// L'utilisation de cette bibliothèque est soumise au droit d'auteur et en 
// particulier aux articles L.122-6 et L.122-6-1 du Code de la propriété 
// intellectuelle. Sa détention, sa reproduction ou sa diffusion sont donc 
// soumis à autorisation préalable de la société. 
//
// Pour tout renseignement : contact@digital-trainers.com

#ifndef SFELITE_DATATYPETRAIT_H
#define SFELITE_DATATYPETRAIT_H

// Globally the same as sofa/defaulttype/DataTypeInfo.h

#include <typeinfo>
#include <type_traits>
#include <string>
#include <sstream>

#include <array>
#include <vector>

namespace sfe
{

class AbstractTypeTrait
{
public:
    virtual const AbstractTypeTrait* BaseType() const = 0;
    virtual const AbstractTypeTrait* ValueType() const = 0;

    virtual bool ValidInfo() const = 0;
    virtual bool FixedSize() const = 0;
    virtual bool SimpleLayout() const = 0;
    virtual bool Integer() const = 0;
    virtual bool Scalar() const = 0;
    virtual bool Text() const = 0;

    virtual size_t size() const = 0;
    virtual size_t byteSize() const = 0;
    virtual size_t size(const void* data) const = 0;
    virtual void setSize(void* data, size_t size) const = 0;

    virtual long long   getIntegerValue(const void* data, size_t index) const = 0;
    virtual double      getScalarValue(const void* data, size_t index) const = 0;
    virtual const std::string& getTextValue(const void* data, size_t index) const = 0;

    virtual void setIntegerValue(void* data, size_t index, long long value) const = 0;
    virtual void setScalarValue(void* data, size_t index, double value) const = 0;
    virtual void setTextValue(void* data, size_t index, const std::string& value) const = 0;

    virtual const void* getValuePtr(const void* type) const = 0;
    virtual void* getValuePtr(void* type) const = 0;

    virtual const std::type_info* type_info() const = 0;

protected: // only derived types can instantiate this class
    AbstractTypeTrait() {}
    virtual ~AbstractTypeTrait() {}

private: // copy constructor or operator forbidden
    AbstractTypeTrait(const AbstractTypeTrait&) {}
    void operator=(const AbstractTypeTrait&) {}
};

template<class TDataType, class dummy = void>
struct DataTypeTrait
{
    typedef TDataType DataType;
    typedef DataType BaseType; ///< class of type contained in DataType
    typedef DataType ValueType; ///< type of the final atomic values
    typedef DataTypeTrait<BaseType> BaseTypeTrait;
    typedef DataTypeTrait<ValueType> ValueTypeTrait;

    enum { ValidInfo       = 0 }; ///< 1 if this type has valid infos
    enum { FixedSize       = 0 }; ///< 1 if this type has a fixed size (setSize does nothing)
    enum { SimpleLayout    = 0 }; ///< 1 if the layout in memory is simply N values of the same base type
    enum { Integer         = 0 }; ///< 1 if this type uses integer values
    enum { Scalar          = 0 }; ///< 1 if this type uses scalar values
    enum { Text            = 0 }; ///< 1 if this type uses text values

    static size_t size() { return 1; }
    static size_t byteSize() { return 1; }
    static size_t size(const DataType& /*data*/) { return 1; }
    static void setSize(DataType& /*data*/, size_t /*size*/) { }

    template <typename T> static void getValue(const DataType& /*data*/, size_t /*index*/, T& /*value*/) { }
    template <typename T> static void setValue(DataType& /*data*/, size_t /*index*/, const T& /*value*/) { }
    static void getValueString(const DataType& /*data*/, size_t /*index*/, std::string& /*value*/) { }
    static void setValueString(DataType& /*data*/, size_t /*index*/, const std::string& /*value*/) { }
    static const void* getValuePtr(const DataType& /*type*/) { return nullptr; }
    static void* getValuePtr(DataType& /*type*/) { return nullptr; }
};

template<class TDataType>
class VirtualTypeTrait : public AbstractTypeTrait
{
public:
    typedef TDataType DataType;
    typedef DataTypeTrait<DataType> Info;

    static VirtualTypeTrait* get() { static VirtualTypeTrait<DataType> t; return &t; }

    virtual const AbstractTypeTrait* BaseType() const  { return VirtualTypeTrait<typename Info::BaseType>::get(); }
    virtual const AbstractTypeTrait* ValueType() const { return VirtualTypeTrait<typename Info::ValueType>::get(); }

    virtual bool ValidInfo() const       { return Info::ValidInfo; }
    virtual bool FixedSize() const       { return Info::FixedSize; }
    virtual bool SimpleLayout() const    { return Info::SimpleLayout; }
    virtual bool Integer() const         { return Info::Integer; }
    virtual bool Scalar() const          { return Info::Scalar; }
    virtual bool Text() const            { return Info::Text; }

    virtual size_t size() const { return Info::size(); }
    virtual size_t byteSize() const { return Info::byteSize(); }
    virtual size_t size(const void* data) const { return Info::size(*(const DataType*)data); }
    virtual void setSize(void* data, size_t size) const { Info::setSize(*(DataType*)data, size); }
    virtual long long getIntegerValue(const void* data, size_t index) const
    { long long v = 0; Info::getValue(*(const DataType*)data, index, v); return v; }
    virtual double getScalarValue(const void* data, size_t index) const
    { double v = 0; Info::getValue(*(const DataType*)data, index, v); return v; }
    virtual const std::string& getTextValue(const void* data, size_t index) const
    { static std::string v; Info::getValueString(*(const DataType*)data, index, v); return v; }
    virtual void setIntegerValue(void* data, size_t index, long long value) const
    { Info::setValue(*(DataType*)data, index, value); }
    virtual void setScalarValue (void* data, size_t index, double value) const
    { Info::setValue(*(DataType*)data, index, value); }
    virtual void setTextValue(void* data, size_t index, const std::string& value) const
    { Info::setValueString(*(DataType*)data, index, value); }
    virtual const void* getValuePtr(const void* data) const { return Info::getValuePtr(*(const DataType*)data); }
    virtual void* getValuePtr(void* data) const { return Info::getValuePtr(*(DataType*)data); }

    virtual const std::type_info* type_info() const { return &typeid(DataType); }

protected: // only derived types can instantiate this class
    VirtualTypeTrait() {}
};

template<class TDataType>
struct PODTypeTrait
{
    typedef TDataType DataType;
    typedef DataType BaseType;
    typedef DataType ValueType;
    typedef PODTypeTrait<DataType> BaseTypeTrait;
    typedef PODTypeTrait<DataType> ValueTypeTrait;

    enum { ValidInfo       = 1 };
    enum { FixedSize       = 1 };
    enum { SimpleLayout    = 1 };
    enum { Integer         = std::is_integral<DataType>::value };
    enum { Scalar          = std::is_scalar<DataType>::value };
    enum { Text            = 0 };

    static size_t size() { return 1; }
    static size_t byteSize() { return sizeof(DataType); }
    static size_t size(const DataType& /*data*/) { return 1; }
    static void setSize(DataType& /*data*/, size_t /*size*/) { }

    template <typename T> static void getValue(const DataType &data, size_t index, T& value)
    { if (index) return; value = static_cast<T>(data); }
    template<typename T> static void setValue(DataType &data, size_t index, const T& value )
    { if (index) return; data = static_cast<DataType>(value); }
    static void getValueString(const DataType &data, size_t index, std::string& value)
    { if (index) return; std::ostringstream o; o << data; value = o.str(); }
    static void setValueString(DataType &data, size_t index, const std::string& value )
    { if (index) return; std::istringstream i(value); i >> data; }
    static const void* getValuePtr(const DataType& data) { return &data; }
    static void* getValuePtr(DataType& data) { return &data; }
};

template<class TDataType>
struct TextTypeTrait
{
    typedef TDataType DataType;
    typedef DataType BaseType;
    typedef DataType ValueType;

    enum { ValidInfo       = 1 };
    enum { FixedSize       = 1 }; // Because setSize does nothing, even if the type itself does change in size
    enum { SimpleLayout    = 0 }; // We will not memcpy strings, but use setValueString
    enum { Integer         = 0 };
    enum { Scalar          = 0 };
    enum { Text            = 1 };

    enum { Size = 1 }; ///< largest known fixed size for this type, as returned by size()
    static size_t size() { return 1; }
    static size_t byteSize() { return 1; }
    static size_t size(const DataType& /*data*/) { return 1; }
    static void setSize(DataType& /*data*/, size_t /*size*/) {  }

    template <typename T> static void getValue(const DataType &data, size_t index, T& value)
    { if (index) return; std::istringstream i(data); i >> value; }
    template<typename T> static void setValue(DataType &data, size_t index, const T& value )
    { if (index) return; std::ostringstream o; o << value; data = o.str(); }
    static void getValueString(const DataType &data, size_t index, std::string& value)
    { if (index) return; value = data; }
    static void setValueString(DataType &data, size_t index, const std::string& value )
    { if (index) return; data = value; }
    static const void* getValuePtr(const DataType& data) { return nullptr; }
    static void* getValuePtr(DataType& data) { return nullptr; }
};

template<class TDataType, size_t static_size, class value_type = typename TDataType::value_type>
struct ArrayTypeTrait
{
    typedef TDataType DataType;
    typedef value_type BaseType;
    typedef DataTypeTrait<BaseType> BaseTypeTrait;
    typedef typename BaseTypeTrait::ValueType ValueType;
    typedef DataTypeTrait<ValueType> ValueTypeTrait;

    enum { ValidInfo       = BaseTypeTrait::ValidInfo       };
    enum { FixedSize       = BaseTypeTrait::FixedSize       };
    enum { SimpleLayout    = BaseTypeTrait::SimpleLayout    };
    enum { Integer         = BaseTypeTrait::Integer         };
    enum { Scalar          = BaseTypeTrait::Scalar          };
    enum { Text            = BaseTypeTrait::Text            };

    enum { Size = static_size };
    static size_t size() { return Size * BaseTypeTrait::size(); }
    static size_t byteSize() { return ValueTypeTrait::byteSize(); }
    static size_t size(const DataType& data) 
    {
        if (FixedSize)
            return size();
        else
        {
            size_t s = 0;
            for (size_t i=0; i<Size; ++i)
                s += BaseTypeTrait::size(data[i]);
            return s;
        }
    }

    static void setSize(DataType& data, size_t size)
    {
        if (!FixedSize)
        {
            size /= Size;
            for (size_t i=0; i<Size; ++i)
                BaseTypeTrait::setSize(data[i], size);
        }
    }

    template <typename T>
    static void getValue(const DataType &data, size_t index, T& value)
    {
        if (BaseTypeTrait::FixedSize)
        {
            if (BaseTypeTrait::size() == 1)
                BaseTypeTrait::getValue(data[index], 0, value);
            else if (BaseTypeTrait::FixedSize)
                BaseTypeTrait::getValue(data[index / BaseTypeTrait::size()], index % BaseTypeTrait::size(), value);
        }
        else
        {
            size_t s = 0;
            for (size_t i = 0; i < Size; ++i)
            {
                size_t n = BaseTypeTrait::size(data[i]);
                if (index < s+n) { BaseTypeTrait::getValue(data[i], index-s, value); break; }
                s += n;
            }
        }
    }

    template<typename T>
    static void setValue(DataType &data, size_t index, const T& value )
    {
        if (BaseTypeTrait::FixedSize)
        {
            if (BaseTypeTrait::size() == 1)
                BaseTypeTrait::setValue(data[index], 0, value);
            else if (BaseTypeTrait::FixedSize)
                BaseTypeTrait::setValue(data[index / BaseTypeTrait::size()], index % BaseTypeTrait::size(), value);
        }
        else
        {
            size_t s = 0;
            for (size_t i = 0; i < Size; ++i)
            {
                size_t n = BaseTypeTrait::size(data[i]);
                if (index < s+n) { BaseTypeTrait::setValue(data[i], index-s, value); break; }
                s += n;
            }
        }
    }

    static void getValueString(const DataType &data, size_t index, std::string& value)
    {
        if (BaseTypeTrait::FixedSize)
        {
            if (BaseTypeTrait::size() == 1)
                BaseTypeTrait::getValueString(data[index], 0, value);
            else if (BaseTypeTrait::FixedSize)
                BaseTypeTrait::getValueString(data[index / BaseTypeTrait::size()], index % BaseTypeTrait::size(), value);
        }
        else
        {
            size_t s = 0;
            for (size_t i = 0; i < Size; ++i)
            {
                size_t n = BaseTypeTrait::size(data[i]);
                if (index < s+n) { BaseTypeTrait::getValueString(data[i], index-s, value); break; }
                s += n;
            }
        }
    }

    static void setValueString(DataType &data, size_t index, const std::string& value )
    {
        if (BaseTypeTrait::FixedSize)
        {
            if (BaseTypeTrait::size() == 1)
                BaseTypeTrait::setValueString(data[index], 0, value);
            else if (BaseTypeTrait::FixedSize)
                BaseTypeTrait::setValueString(data[index / BaseTypeTrait::size()], index % BaseTypeTrait::size(), value);
        }
        else
        {
            size_t s = 0;
            for (size_t i = 0; i < Size; ++i)
            {
                size_t n = BaseTypeTrait::size(data[i]);
                if (index < s+n) { BaseTypeTrait::setValueString(data[i], index-s, value); break; }
                s += n;
            }
        }
    }

    static const void* getValuePtr(const DataType& data)
    { return &data[0]; }
    static void* getValuePtr(DataType& data)
    { return &data[0]; }
};

template<class TDataType>
struct VectorTypeTrait
{
    typedef TDataType DataType;
    typedef typename DataType::value_type BaseType;
    typedef DataTypeTrait<BaseType> BaseTypeTrait;
    typedef typename BaseTypeTrait::ValueType ValueType;
    typedef DataTypeTrait<ValueType> ValueTypeTrait;

    enum { ValidInfo       = BaseTypeTrait::ValidInfo       };
    enum { FixedSize       = 0                              };
    enum { SimpleLayout    = BaseTypeTrait::SimpleLayout    };
    enum { Integer         = BaseTypeTrait::Integer         };
    enum { Scalar          = BaseTypeTrait::Scalar          };
    enum { Text            = BaseTypeTrait::Text            };

    static size_t size() { return BaseTypeTrait::size(); }
    static size_t byteSize() { return ValueTypeTrait::byteSize(); }

    static size_t size(const DataType& data)
    {
        if (BaseTypeTrait::FixedSize)
            return data.size() * BaseTypeTrait::size();
        else
        {
            size_t s = 0;
            for (size_t i=0, n = data.size(); i<n; ++i)
                s+= BaseTypeTrait::size(data[i]);
            return s;
        }
    }

    static void setSize(DataType& data, size_t size)
    {
        if (BaseTypeTrait::FixedSize)
            data.resize(size/BaseTypeTrait::size());
    }

    template <typename T>
    static void getValue(const DataType &data, size_t index, T& value)
    {
        if (BaseTypeTrait::FixedSize)
        {
            if (BaseTypeTrait::size() == 1)
                BaseTypeTrait::getValue(data[index], 0, value);
            else if (BaseTypeTrait::FixedSize)
                BaseTypeTrait::getValue(data[index / BaseTypeTrait::size()], index % BaseTypeTrait::size(), value);
        }
        else
        {
            size_t s = 0;
            for (size_t i = 0, nb = data.size(); i < nb; ++i)
            {
                size_t n = BaseTypeTrait::size(data[i]);
                if (index < s+n) { BaseTypeTrait::getValue(data[i], index-s, value); break; }
                s += n;
            }
        }
    }

    template<typename T>
    static void setValue(DataType &data, size_t index, const T& value )
    {
        if (BaseTypeTrait::FixedSize)
        {
            if (BaseTypeTrait::size() == 1)
                BaseTypeTrait::setValue(data[index], 0, value);
            else if (BaseTypeTrait::FixedSize)
                BaseTypeTrait::setValue(data[index / BaseTypeTrait::size()], index % BaseTypeTrait::size(), value);
        }
        else
        {
            size_t s = 0;
            for (size_t i = 0, nb = data.size(); i < nb; ++i)
            {
                size_t n = BaseTypeTrait::size(data[i]);
                if (index < s+n) { BaseTypeTrait::setValue(data[i], index-s, value); break; }
                s += n;
            }
        }
    }

    static void getValueString(const DataType &data, size_t index, std::string& value)
    {
        if (BaseTypeTrait::FixedSize)
        {
            if (BaseTypeTrait::size() == 1)
                BaseTypeTrait::getValueString(data[index], 0, value);
            else if (BaseTypeTrait::FixedSize)
                BaseTypeTrait::getValueString(data[index / BaseTypeTrait::size()], index % BaseTypeTrait::size(), value);
        }
        else
        {
            size_t s = 0;
            for (size_t i = 0, nb = data.size(); i < nb; ++i)
            {
                size_t n = BaseTypeTrait::size(data[i]);
                if (index < s+n) { BaseTypeTrait::getValueString(data[i], index-s, value); break; }
                s += n;
            }
        }
    }

    static void setValueString(DataType &data, size_t index, const std::string& value )
    {
        if (BaseTypeTrait::FixedSize)
        {
            if (BaseTypeTrait::size() == 1)
                BaseTypeTrait::setValueString(data[index], 0, value);
            else if (BaseTypeTrait::FixedSize)
                BaseTypeTrait::setValueString(data[index / BaseTypeTrait::size()], index % BaseTypeTrait::size(), value);
        }
        else
        {
            size_t s = 0;
            for (size_t i = 0, nb = data.size(); i < nb; ++i)
            {
                size_t n = BaseTypeTrait::size(data[i]);
                if (index < s+n) { BaseTypeTrait::setValueString(data[i], index-s, value); break; }
                s += n;
            }
        }
    }

    static const void* getValuePtr(const DataType& data) { return &data[0]; }
    static void* getValuePtr(DataType& data) { return &data[0]; }
};

template <class T>
struct DataTypeTrait<T, typename std::enable_if<std::is_arithmetic<T>::value>::type> : public PODTypeTrait<T> 
{ };

template <>
struct DataTypeTrait<bool> : public PODTypeTrait<bool>
{
    template<typename T> static void setValue(DataType& data, size_t index, const T& value)
    { if (index) return; data = (value != 0); }
    template<typename T> static void setValue(std::vector<bool>::reference data, size_t index, const T& value)
    { if (index) return; data = (value != 0); }
    static void setValueString(DataType &data, size_t index, const std::string& value)
    { if (index) return; std::istringstream i(value); i >> data; }
    static void setValueString(std::vector<bool>::reference data, size_t index, const std::string& value)
    { if (index) return; bool b = data; std::istringstream i(value); i >> b; data = b; }
};

template <>
struct DataTypeTrait<std::string> : public TextTypeTrait<std::string>
{ };

template <class T, std::size_t N>
struct DataTypeTrait<std::array<T, N>> : public ArrayTypeTrait<std::array<T, N>, N> 
{ };

template<class T, class Alloc>
struct DataTypeTrait<std::vector<T, Alloc>> : public VectorTypeTrait<std::vector<T, Alloc>> 
{ };

template<class Alloc>
struct DataTypeTrait<std::vector<bool, Alloc>> : public VectorTypeTrait<std::vector<bool, Alloc>>
{
    enum { SimpleLayout = 0 };

    static const void* getValuePtr(const std::vector<bool, Alloc>& data) { return nullptr; }
    static void* getValuePtr(std::vector<bool, Alloc>& data) { return nullptr; }
};

template <class T>
std::string toString(const T& val)
{
    auto info = sfe::VirtualTypeTrait<T>::get();
    size_t size = info->size(&val);
    std::ostringstream out;

    if (size)
        out << info->getTextValue(&val, 0);

    for (size_t i = 1; i < size; ++i)
        out << " " << info->getTextValue(&val, i);

    return out.str();
}

} // namespace sfe

#endif // SFELITE_DATATYPETRAIT_H
