#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <cstdint>
#include <string>
#include <sstream>
#include <tuple>
#include <glm/gtx/string_cast.hpp>

namespace FieaGameEngine
{
	/// <summary>Forward declaration of Scope class.</summary>
	class Scope;
	/// <summary>Datum - Runtime Polymorphic Data Type.</summary>
	class Datum final
	{
	public:
		/// <summary>Strong (Scoped) enumeration representing Data Types supported by Datum.</summary>
		enum class DatumType
		{
			UNKNOWN,
			INTEGER,
			FLOAT,
			VECTOR4,
			MATRIX4X4,
			TABLE,
			STRING,
			POINTER
		};

		/// <summary>Storage types which are used by Datum.</summary>
		enum class DatumStorageType
		{
			UNKNOWN,
			INTERNAL,
			EXTERNAL
		};

		/// <summary>Default Constructor - Initialize Datum to be empty. No memory is allocated.</summary>
		explicit Datum(DatumType t_type = DatumType::UNKNOWN);

		/// <summary>Copy Constructor - Construct a Datum object using another object passed as reference.</summary>
		/// <param name="t_rhs">Const reference to passed Datum</param>
		Datum(const Datum& t_rhs);

		/// <summary>Move Constructor for Datum</summary>
		/// <param name="t_rhs">R-Value reference to a passed Datum which is to be moved.</param>
		Datum(Datum&& t_rhs);

		/// <summary>Copy Assignment Operator for Datum. Makes Deep copy of passed Datum.</summary>
		/// <param name="t_rhs">Const reference to passed Datum.</param>
		/// <returns>Returns reference to current Datum</returns>
		Datum& operator=(const Datum& t_rhs);

		/// <summary>Move Assignment Operator for Datum.</summary>
		/// <param name="t_rhs">R-Value reference to a passed Datum which is to be moved.</param>
		/// <returns>Returns reference to current Datum.</returns>
		Datum& operator=(Datum&& t_rhs);

		/// <summary>Datum - Assignment Operator overload for 32-Bit Integer.</summary>
		/// <param name="t_rhs">Const reference to an 32-Bit Integer.</param>
		/// <returns>Returns reference to current Datum.</returns>
		Datum& operator=(const int32_t& t_rhs);

		/// <summary>Datum - Assignment Operator overload for float.</summary>
		/// <param name="t_rhs">Const reference to an 32-Bit Integer.</param>
		/// <returns>Returns reference to current Datum.</returns>
		Datum& operator=(const float_t& t_rhs);

		/// <summary>Datum - Assignment Operator overload for OpenGL standard 4 component vector.</summary>
		/// <param name="t_rhs">Const reference to an 4 components based single-precision floating point vector.</param>
		/// <returns>Returns reference to current Datum.</returns>
		Datum& operator=(const glm::vec4& t_rhs);

		/// <summary>Datum - Assignment Operator overload for OpenGL standard 4x4 matrix of floating point numbers.</summary>
		/// <param name="t_rhs">Const reference to an 4x4 components matrix structure of single-precision floating point numbers.</param>
		/// <returns>Returns reference to current Datum.</returns>
		Datum& operator=(const glm::mat4x4& t_rhs);

		/// <summary>Datum - Assignment Operator overload for String.</summary>
		/// <param name="t_rhs">Const reference to a string.</param>
		/// <returns>Returns reference to current Datum.</returns>
		Datum& operator=(const std::string& t_rhs);

		/// <summary>Datum - Assignment Operator overload for RTTI member reference</summary>
		/// <param name="t_rhs">pointer to RTTI class</param>
		/// <returns>Returns reference to current Datum.</returns>
		Datum& operator=(RTTI* t_rhs);

		/// <summary>Datum - Assignment Operator overload for Scope member reference</summary>
		/// <param name="t_rhs">Reference to a Scope class</param>
		/// <returns>Returns reference to current Datum.</returns>
		Datum& operator=(Scope& t_rhs);

		/// <summary>Destructor for Datum. Free any memory owned by this object.</summary>
		~Datum();

		/// <summary>Get Type enumeration associated with this object.</summary>
		/// <returns>Returns Type enum associated with this object.</returns>
		DatumType type() const;

		/// <summary>Given Type enumeration, assign the type of data this object stores.</summary>
		/// <param name="t_type">Enumeration of class DatumType.</param>
		void setType(DatumType t_type);

		/// <summary>Get number of elements stored in the Datum.</summary>
		/// <returns>Returns unsigned integer no. of values.</returns>
		uint32_t size() const;

		/// <summary>Get total capacity ( max. number of elements ) which can be stored in a Datum.</summary>
		/// <returns>Returns capacity in unsigned integer.</returns>
		uint32_t capacity() const;

		/// <summary>Get Storage type of given datum. ( Unknown, Internal or External ).</summary>
		/// <returns>Returns Enumeration of DatumStorageType indicating type of storage used for datum.</returns>
		DatumStorageType storageType() const;

		/// <summary>To get the Storage type status of a datum ( whether its external or not ).</summary>
		/// <returns>Returns a boolean value indicating whether datum has external storage or not.</returns>
		bool isExternal() const;

		/// <summary>
		/// Set no. of values and reserve memory if needed. If passed size is less than current size, destruct elements.
		/// Doesn't shrink the capacity in that case. 
		/// </summary>
		/// <param name="t_size">Number of values ( size ) in Unsigned Integer.</param>
		void setSize(uint32_t t_size);

		/// <summary>Clear Datum object ( Array of Elements ) without shrinking the capacity. Only works if Storage Type is INTERNAL.</summary>
		void clear();

		/// <summary>Given a new capacity, allocate / reallocate array elements to new position in memory.</summary>
		/// <param name="t_new_capacity">Capacity of new array to which current array to be expanded.</param>
		void reserve(uint32_t t_new_capacity);

		/// <summary>Assign internal values Datum type to passed Datum type. Set size and capacity to given number of elements.</summary>
		/// <param name="t_type">Const reference to a passed Datum Type</param>
		/// <param name="t_array_size">Number of elements in passed array.</param>
		void setStorage(const DatumType& t_type, uint32_t t_array_size);

		/// <summary>Assign internal values array to passed integer array. Set size and capacity to given number of elements.</summary>
		/// <param name="t_array">Pointer to a passed float array of elements.</param>
		/// <param name="t_array_size">Number of elements in passed array.</param>
		void setStorage(int32_t* t_array, uint32_t t_array_size);

		/// <summary>Assign internal values array to passed float array. Set size and capacity to given number of elements.</summary>
		/// <param name="t_array">Pointer to a passed Array of elements.</param>
		/// <param name="t_array_size">Number of elements in passed array.</param>
		void setStorage(float_t* t_array, uint32_t t_array_size);

		/// <summary>Assign internal values array to passed OpenGL standard 4-components vector array. Set size and capacity to given number of elements.</summary>
		/// <param name="t_array">Pointer to a passed 4-components single precision floating point numbers array of elements.</param>
		/// <param name="t_array_size">Number of elements in passed array.</param>
		void setStorage(glm::vec4* t_array, uint32_t t_array_size);

		/// <summary>Assign internal values array to array of OpenGL standard 4x4 floating point numbers Matrix. Set size and capacity to given number of elements.</summary>
		/// <param name="t_array">Pointer to a passed 4x4 components matrix structure of single-precision floating point numbers array of elements.</param>
		/// <param name="t_array_size">Number of elements in passed array.</param>
		void setStorage(glm::mat4x4* t_array, uint32_t t_array_size);

		/// <summary>Assign internal values array to passed string array. Set size and capacity to given number of elements.</summary>
		/// <param name="t_array">Pointer to a passed string Array of elements.</param>
		/// <param name="t_array_size">Number of elements in passed array.</param>
		void setStorage(std::string* t_array, uint32_t t_array_size);

		/// <summary>Assign internal values array to passed RTTI reference array. Set size and capacity to given number of elements.</summary>
		/// <param name="t_array">Pointer to RTTI class pointer</param>
		/// <param name="t_array_size">Number of elements in passed array.</param>
		void setStorage(RTTI** t_array, uint32_t t_array_size);


		void setStorage(void* t_storage, uint32_t t_size);


		/// <summary>Comparison operator overload for two Datums.</summary>
		/// <param name="t_rhs">Const reference to passed Datum</param>
		/// <returns>Returns boolean result indicating whether two Datums are equal or not.</returns>
		bool operator==(const Datum& t_rhs) const;

		/// <summary>Comparison operator overload to compare Datum Value against passed integer value.</summary>
		/// <param name="t_rhs">Const reference to passed Integer</param>
		/// <returns>Returns boolean result indicating whether two Datums are equal or not.</returns>
		bool operator==(const int32_t& t_rhs) const;

		/// <summary>Comparison operator overload to compare Datum Value against passed float value.</summary>
		/// <param name="t_rhs">Const reference to passed Float</param>
		/// <returns>Returns boolean result indicating whether two Datums are equal or not.</returns>
		bool operator==(const float_t& t_rhs) const;

		/// <summary>Comparison operator overload to compare Datum Value against passed OpenGL standard 4-components vector value.</summary>
		/// <param name="t_rhs">Const reference to passed 4-components single precision floating point numbers Vector</param>
		/// <returns>Returns boolean result indicating whether two Datums are equal or not.</returns>
		bool operator==(const glm::vec4& t_rhs) const;

		/// <summary>Comparison operator overload to compare Datum Value against passed OpenGL standard 4x4 matrix value.</summary>
		/// <param name="t_rhs">Const reference to passed 4x4 matrix of single precision floating point numbers</param>
		/// <returns>Returns boolean result indicating whether two Datums are equal or not.</returns>
		bool operator==(const glm::mat4x4& t_rhs) const;

		/// <summary>Comparison operator overload to compare Datum Value against passed string value.</summary>
		/// <param name="t_rhs">Const reference to passed string</param>
		/// <returns>Returns boolean result indicating whether two Datums are equal or not.</returns>
		bool operator==(const std::string& t_rhs) const;

		/// <summary>Comparison operator overload to compare Datum Value against passed RTTI reference value.</summary>
		/// <param name="t_rhs">Const pointer to RTTI class</param>
		/// <returns>Returns boolean result indicating whether two Datums are equal or not.</returns>
		bool operator==(const RTTI* t_rhs) const;

		/// <summary>Comparison operator overload to compare Datum Value against passed Scope pointer value.</summary>
		/// <param name="t_rhs">Const pointer to Scope class</param>
		/// <returns>Returns boolean result indicating whether two Datums are equal or not.</returns>
		bool operator==(const Scope* t_rhs) const;

		/// <summary>Not-Equal operator overload for two Datums.</summary>
		/// <param name="t_rhs">Const reference to passed Datum</param>
		/// <returns>Returns boolean result indicating whether two Datums are equal or not.</returns>
		bool operator!=(const Datum& t_rhs) const;

		/// <summary>Not-Equal operator overload to compare Datum Value against passed integer value.</summary>
		/// <param name="t_rhs">Const reference to passed Integer</param>
		/// <returns>Returns boolean result indicating whether two Datums are equal or not.</returns>
		bool operator!=(const int32_t& t_rhs) const;

		/// <summary>Not-Equal operator overload to compare Datum Value against passed float value.</summary>
		/// <param name="t_rhs">Const reference to passed Float</param>
		/// <returns>Returns boolean result indicating whether two Datums are equal or not.</returns>
		bool operator!=(const float_t& t_rhs) const;

		/// <summary>Not-Equal operator overload to compare Datum Value against passed OpenGL standard 4-components vector value.</summary>
		/// <param name="t_rhs">Const reference to passed 4-components single precision floating point numbers Vector</param>
		/// <returns>Returns boolean result indicating whether two Datums are equal or not.</returns>
		bool operator!=(const glm::vec4& t_rhs) const;

		/// <summary>Not-Equal operator overload to compare Datum Value against passed OpenGL standard 4x4 matrix value.</summary>
		/// <param name="t_rhs">Const reference to passed 4x4 matrix of single precision floating point numbers</param>
		/// <returns>Returns boolean result indicating whether two Datums are equal or not.</returns>
		bool operator!=(const glm::mat4x4& t_rhs) const;

		/// <summary>Not-Equal operator overload to compare Datum Value against passed string value.</summary>
		/// <param name="t_rhs">Const reference to passed string</param>
		/// <returns>Returns boolean result indicating whether two Datums are equal or not.</returns>
		bool operator!=(const std::string& t_rhs) const;

		/// <summary>Not-Equal operator overload to compare Datum Value against passed RTTI pointer value.</summary>
		/// <param name="t_rhs">Const pointer to RTTI class</param>
		/// <returns>Returns boolean result indicating whether two Datums are equal or not.</returns>
		bool operator!=(const RTTI* t_rhs) const;

		/// <summary>Not-Equal operator overload to compare Datum Value against passed Scope pointer value.</summary>
		/// <param name="t_rhs">Const pointer to Scope class</param>
		/// <returns>Returns boolean result indicating whether two Datums are equal or not.</returns>
		bool operator!=(const Scope* t_rhs) const;

		/// <summary>
		/// Assign appropriate element to array at given index.
		/// If passed Index is greater than size, exception is thrown.
		/// </summary>
		/// <param name="t_type">Data Type Enumeration for passed value </param>
		/// <param name="t_index">(Optional) Index of the given value. Default is 0.</param>
		void set(const DatumType& t_type, uint32_t t_index = 0);

		/// <summary>
		/// Assign appropriate element to array at given index.
		/// If passed Index is greater than size, exception is thrown.
		/// </summary>
		/// <param name="t_value">Const Reference to an Integer value</param>
		/// <param name="t_index">(Optional) Index of the given value. Default is 0.</param>
		void set(const int32_t& t_value, uint32_t t_index = 0);

		/// <summary>
		/// Assign appropriate element to array at given index.
		/// If passed Index is greater than size, exception is thrown.
		/// </summary>
		/// <param name="t_value">Const Reference to a Float value</param>
		/// <param name="t_index">(Optional) Index of the given value. Default is 0.</param>
		void set(const float_t& t_value, uint32_t t_index = 0);

		/// <summary>
		/// Assign appropriate element to array at given index.
		/// If passed Index is greater than size, exception is thrown.
		/// </summary>
		/// <param name="t_value">Const Reference to OpenGL standard 4-components vector</param>
		/// <param name="t_index">(Optional) Index of the given value. Default is 0.</param>
		void set(const glm::vec4& t_value, uint32_t t_index = 0);

		/// <summary>
		/// Assign appropriate element to array at given index.
		/// If passed Index is greater than size, exception is thrown.
		/// </summary>
		/// <param name="t_value">Const Reference to OpenGL standard 4x4 matrix of floating point numbers.</param>
		/// <param name="t_index">(Optional) Index of the given value. Default is 0.</param>
		void set(const glm::mat4x4& t_value, uint32_t t_index = 0);

		/// <summary>
		/// Assign appropriate element to array at given index.
		/// If passed Index is greater than size, exception is thrown.
		/// </summary>
		/// <param name="t_value">Const Reference to a std::string value</param>
		/// <param name="t_index">(Optional) Index of the given value. Default is 0.</param>
		void set(const std::string& t_value, uint32_t t_index = 0);

		/// <summary>
		/// Assign appropriate element to array at given index.
		/// If passed Index is greater than size, exception is thrown.
		/// </summary>
		/// <param name="t_value">Pointer to a RTTI class</param>
		/// <param name="t_index">(Optional) Index of the given value. Default is 0.</param>
		void set(RTTI* t_value, uint32_t t_index = 0);

		/// <summary>
		/// Assign appropriate element to array at given index.
		/// If passed Index is greater than size, exception is thrown.
		/// </summary>
		/// <param name="t_value">Pointer to a Scope class</param>
		/// <param name="t_index">(Optional) Index of the given value. Default is 0.</param>
		void set(Scope* t_value, uint32_t t_index = 0);


		/// <summary>For a given index, return appropriate value from array.</summary>
		/// <param name="t_index">(Optional) Index of the value to be returned. Default is 0.</param>
		/// <returns>Returns reference of type T</returns>
		template <typename T>
		T& get(uint32_t t_index = 0);

		/// <summary>
		/// get() - Template Specialisation (int32_t)
		/// For a given index, return appropriate value from array.
		/// </summary>
		/// <param name="t_index">index of an element as an Unsigned integer ( 32-bit ).</param>
		/// <returns>Returns reference to an Unsigned integer ( 32-bit ).</returns>
		template<> int32_t& get(uint32_t t_index);

		/// <summary>
		/// get() - Template Specialisation (float_t)
		/// For a given index, return appropriate value from array.
		/// </summary>
		/// <param name="t_index">index of an element as an Unsigned integer ( 32-bit ).</param>
		/// <returns>Returns reference to an Unsigned integer ( 32-bit ).</returns>
		template<> float_t& get(uint32_t t_index);

		/// <summary>
		/// get() - Template Specialisation (glm::vec4)
		/// For a given index, return appropriate value from array.
		/// </summary>
		/// <param name="t_index">index of an element as an Unsigned integer ( 32-bit ).</param>
		/// <returns>Returns reference to an OpenGL standard 4 components vector.</returns>
		template<> glm::vec4& get(uint32_t t_index);

		/// <summary>
		/// get() - Template Specialisation (glm::mat4x4)
		/// For a given index, return appropriate value from array.
		/// </summary>
		/// <param name="t_index">index of an element as an Unsigned integer ( 32-bit ).</param>
		/// <returns>Returns reference to a OpenGL standard 4x4 matrices of floating point numbers.</returns>
		template<> glm::mat4x4& get(uint32_t t_index);

		/// <summary>
		/// get() - Template Specialisation (std::string)
		/// For a given index, return appropriate value from array.
		/// </summary>
		/// <param name="t_index">index of an element as an Unsigned integer ( 32-bit ).</param>
		/// <returns>Returns reference to a std::string.</returns>
		template<> std::string& get(uint32_t t_index);

		/// <summary>
		/// get() - Template Specialisation (RTTI)
		/// For a given index, return appropriate value from array.
		/// </summary>
		/// <param name="t_index">index of an element as an Unsigned integer ( 32-bit ).</param>
		/// <returns>Returns reference to a RTTI pointer.</returns>
		template<> RTTI*& get(uint32_t t_index);

		/// <summary>
		/// get() - Template Specialisation (Scope)
		/// For a given index, return appropriate value from array.
		/// </summary>
		/// <param name="t_index">index of an element as an Unsigned integer ( 32-bit ).</param>
		/// <returns>Returns reference to a Scope pointer.</returns>
		template<> Scope*& get<Scope*>(uint32_t t_index);


		/// <summary>Const version of get() - For a given index, return appropriate value from array.</summary>
		/// <param name="t_index">(Optional) Index of the value to be returned. Default is 0.</param>
		/// <returns>Returns const reference of type T</returns>
		template <typename T>
		const T& get(uint32_t t_index = 0) const;

		/// <summary>
		/// Const version of get() - Template Specialisation (int32_t)
		/// For a given index, return appropriate value from array.
		/// </summary>
		/// <param name="t_index">index of an element as an Unsigned integer ( 32-bit ).</param>
		/// <returns>Returns const reference to an Unsigned integer ( 32-bit ).</returns>
		template<> const int32_t& get(uint32_t t_index) const;

		/// <summary>
		/// Const version of get() - Template Specialisation (float_t)
		/// For a given index, return appropriate value from array.
		/// </summary>
		/// <param name="t_index">index of an element as an Unsigned integer ( 32-bit ).</param>
		/// <returns>Returns const reference to an Unsigned integer ( 32-bit ).</returns>
		template<> const float_t& get(uint32_t t_index) const;

		/// <summary>
		/// Const version of get() - Template Specialisation (glm::vec4)
		/// For a given index, return appropriate value from array.
		/// </summary>
		/// <param name="t_index">index of an element as an Unsigned integer ( 32-bit ).</param>
		/// <returns>Returns const reference to an OpenGL standard 4 components vector.</returns>
		template<> const glm::vec4& get(uint32_t t_index) const;

		/// <summary>
		/// Const version of get() - Template Specialisation (glm::mat4x4)
		/// For a given index, return appropriate value from array.
		/// </summary>
		/// <param name="t_index">index of an element as an Unsigned integer ( 32-bit ).</param>
		/// <returns>Returns const reference to a OpenGL standard 4x4 matrices of floating point numbers.</returns>
		template<> const glm::mat4x4& get(uint32_t t_index) const;

		/// <summary>
		/// Const version of get() - Template Specialisation (std::string)
		/// For a given index, return appropriate value from array.
		/// </summary>
		/// <param name="t_index">index of an element as an Unsigned integer ( 32-bit ).</param>
		/// <returns>Returns const reference to a std::string.</returns>
		template<> const std::string& get(uint32_t t_index) const;

		/// <summary>
		/// Const version of get() - Template Specialisation (RTTI)
		/// For a given index, return appropriate value from array.
		/// </summary>
		/// <param name="t_index">index of an element as an Unsigned integer ( 32-bit ).</param>
		/// <returns>Returns const reference to a RTTI pointer.</returns>
		template<> RTTI* const& get(uint32_t t_index) const;

		/// <summary>
		/// Const version of get() - Template Specialisation (Scope)
		/// For a given index, return appropriate value from array.
		/// </summary>
		/// <param name="t_index">index of an element as an Unsigned integer ( 32-bit ).</param>
		/// <returns>Returns const reference to a Scope pointer.</returns>
		template<> Scope* const& get<Scope*>(uint32_t t_index) const;


		/// <summary>Set Datum values directly from the given STL string. (Data type of Datum must have to be set before calling this function ).</summary>
		/// <param name="t_input_string">Const reference to passed STL string which is parsed.</param>
		/// <param name="t_index">(Optional) index of array where parsed element should be placed.</param>
		void setFromString(const std::string& t_input_string, uint32_t t_index = 0);

		/// <summary>Given an optional index, return an STL string representing the appropriate element in the values array.</summary>
		/// <param name="t_index">(Optional) index of position in array whose data should be returned in STL string format.</param>
		/// <returns>Returns STL string representation of Data.</returns>
		std::string toString(uint32_t t_index = 0);

		/// <summary>Push Back function For Integer (uint32_t) datatype. Append data to the end of the array.</summary>
		/// <param name="t_item"> Const reference to an passed Integer data.</param>
		void pushBack(const int32_t& t_item);

		/// <summary>Push Back function For Float (float_t) datatype. Append data to the end of the array.</summary>
		/// <param name="t_item"> Const reference to a passed Float data.</param>
		void pushBack(const float_t& t_item);

		/// <summary>Push Back function For OpenGL Vector (glm::vec4) datatype. Append data to the end of the array.</summary>
		/// <param name="t_item"> Const reference to a passed OpenGL vector of 4 components.</param>
		void pushBack(const glm::vec4& t_item);

		/// <summary>Push Back function For OpenGL 4x4 Matrices (glm::mat4x4) datatype. Append data to the end of the array.</summary>
		/// <param name="t_item"> Const reference to a passed OpenGL 4x4 matrices of single precision floating point data.</param>
		void pushBack(const glm::mat4x4& t_item);

		/// <summary>Push Back function For STL String (std::string) datatype. Append data to the end of the array.</summary>
		/// <param name="t_item"> Const reference to a passed STL string.</param>
		void pushBack(const std::string& t_item);

		/// <summary>Push Back function For RTTI (RTTI*) datatype. Append data to the end of the array.</summary>
		/// <param name="t_item">Pointer to a RTTI class.</param>
		void pushBack(RTTI* t_item);

		/// <summary>Push Back function For Scope (Scope*) datatype. Append data to the end of the array.</summary>
		/// <param name="t_item">Pointer to a Scope class.</param>
		void pushBack(Scope* t_item);

		/// <summary>Remove last element from the values array of Datum.</summary>
		void popBack();

		/// <summary>Get first element from the values array of Datum.</summary>
		/// <param name="placeholder">Placeholder of Unsigned int ( 32-bit ). Doesn't serve any purpose.</param>
		/// <returns>Returns Non-const reference to Type T.</returns>
		template <typename T>
		T& front(uint32_t placeholder = 0);

		/// <summary>Const version of find() - Get first element from the values array of Datum.</summary>
		/// <param name="placeholder">Placeholder of Unsigned int ( 32-bit ). Doesn't serve any purpose.</param>
		/// <returns>Returns Const reference to Type T.</returns>
		template <typename T>
		const T& front(uint32_t placeholder = 0) const;

		/// <summary>Get last element from the values array of Datum.</summary>
		/// <param name="placeholder">Placeholder of Unsigned int ( 32-bit ). Doesn't serve any purpose.</param>
		/// <returns>Returns Non-const reference to Type T.</returns>
		template <typename T>
		T& back(uint32_t placeholder = 0);

		/// <summary>Get last element from the values array of Datum.</summary>
		/// <param name="placeholder">Placeholder of Unsigned int ( 32-bit ). Doesn't serve any purpose.</param>
		/// <returns>Returns Non-const reference to Type T.</returns>
		template <typename T>
		const T& back(uint32_t placeholder = 0) const;

		/// <summary>Remove given element from the Datum. If element isn't found in the Datum, does nothing.</summary>
		/// <param name="t_item">Const reference to an element of Type Integer ( 32-Bit ).</param>
		bool remove(const int32_t& t_item);

		/// <summary>Remove given element from the Datum. If element isn't found in the Datum, does nothing.</summary>
		/// <param name="t_item">Const reference to anelement of Type Float ( 32-Bit ).</param>
		bool remove(const float_t& t_item);

		/// <summary>Remove given element from the Datum. If element isn't found in the Datum, does nothing.</summary>
		/// <param name="t_item">Const reference to OpenGL standard 4-Components floating point Vector.</param>
		bool remove(const glm::vec4& t_item);

		/// <summary>Remove given element from the Datum. If element isn't found in the Datum, does nothing.</summary>
		/// <param name="t_item">Const reference to OpenGL standard 4x4 Matrix structure of single precision floating point numbers.</param>
		bool remove(const glm::mat4x4& t_item);

		/// <summary>Remove given element from the Datum. If element isn't found in the Datum, does nothing.</summary>
		/// <param name="t_item">Const reference to an element of Type STL string ( std::string )</param>
		bool remove(const std::string& t_item);

		/// <summary>Remove given element from the Datum. If element isn't found in the Datum, does nothing.</summary>
		/// <param name="t_item">Const reference to RTTI class.</param>
		bool remove(const RTTI& t_item);

		bool remove(const Scope& t_item);

		/// <summary>Remove element at specified index from the Datum. If element isn't found in the Datum, does nothing.</summary>
		/// <param name="t_index">Unsigned integer of 32-Bit</param>
		bool removeAt(uint32_t t_index);

		/// <summary>Search Given element in the values array of Datum.</summary>
		/// <param name="t_item">Const reference to an element of Type Unsigned Integer ( 32-Bit )</param>
		/// <returns>Returns std::tuple of boolean result & index of an element. Index will be size of an array if element isn't found.</returns>
		std::tuple<bool, uint32_t> find(const int32_t& t_item);

		/// <summary>Search Given element in the values array of Datum.</summary>
		/// <param name="t_item">Const reference to an element of Type Float.</param>
		/// <returns>Returns std::tuple of boolean result & index of an element. Index will be size of an array if element isn't found.</returns>
		std::tuple<bool, uint32_t> find(const float_t& t_item);

		/// <summary>Search Given element in the values array of Datum.</summary>
		/// <param name="t_item">Const reference to an OpenGL standard 4-Components floating point Vector.</param>
		/// <returns>Returns std::tuple of boolean result & index of an element. Index will be size of an array if element isn't found.</returns>
		std::tuple<bool, uint32_t> find(const glm::vec4& t_item);

		/// <summary>Search Given element in the values array of Datum.</summary>
		/// <param name="t_item">Const reference to an OpenGL standard 4x4 Matrix of Single Precision floating point numbers.</param>
		/// <returns>Returns std::tuple of boolean result & index of an element. Index will be size of an array if element isn't found.</returns>
		std::tuple<bool, uint32_t> find(const glm::mat4x4& t_item);

		/// <summary>Search Given element in the values array of Datum.</summary>
		/// <param name="t_item">Const reference to a STL string ( std::string ).</param>
		/// <returns>Returns std::tuple of boolean result & index of an element. Index will be size of an array if element isn't found.</returns>
		std::tuple<bool, uint32_t> find(const std::string& t_item);

		/// <summary>Search Given element in the values array of Datum.</summary>
		/// <param name="t_item">Const reference to a RTTI class</param>
		/// <returns>Returns std::tuple of boolean result & index of an element. Index will be size of an array if element isn't found.</returns>
		std::tuple<bool, uint32_t> find(const RTTI& t_item);

		/// <summary>Search Given element in the values array of Datum.</summary>
		/// <param name="t_item">Const reference to a Scope</param>
		/// <returns>Returns std::tuple of boolean result & index of an element. Index will be size of an array if element isn't found.</returns>
		std::tuple<bool, uint32_t> find(const Scope& t_item);

		/// <summary>Const version of find() - Search Given element in the values array of Datum.</summary>
		/// <param name="t_item">Const reference to an element of Type Unsigned Integer ( 32-Bit )</param>
		/// <returns>Returns const std::tuple of boolean result & index of an element. Index will be size of an array if element isn't found.</returns>
		const std::tuple<bool, uint32_t> find(const int32_t& t_item) const;

		/// <summary>Const version of find() - Search Given element in the values array of Datum.</summary>
		/// <param name="t_item">Const reference to an element of Type Float.</param>
		/// <returns>Returns const std::tuple of boolean result & index of an element. Index will be size of an array if element isn't found.</returns>
		const std::tuple<bool, uint32_t> find(const float_t& t_item) const;

		/// <summary>Const version of find() - Search Given element in the values array of Datum.</summary>
		/// <param name="t_item">Const reference to an OpenGL standard 4-Components floating point Vector.</param>
		/// <returns>Returns const std::tuple of boolean result & index of an element. Index will be size of an array if element isn't found.</returns>
		const std::tuple<bool, uint32_t> find(const glm::vec4& t_item) const;

		/// <summary>Const version of find() - Search Given element in the values array of Datum.</summary>
		/// <param name="t_item">Const reference to an OpenGL standard 4x4 Matrix of Single Precision floating point numbers.</param>
		/// <returns>Returns const std::tuple of boolean result & index of an element. Index will be size of an array if element isn't found.</returns>
		const std::tuple<bool, uint32_t> find(const glm::mat4x4& t_item) const;

		/// <summary>Const version of find() - Search Given element in the values array of Datum.</summary>
		/// <param name="t_item">Const reference to a STL string ( std::string ).</param>
		/// <returns>Returns const std::tuple of boolean result & index of an element. Index will be size of an array if element isn't found.</returns>
		const std::tuple<bool, uint32_t> find(const std::string& t_item) const;

		/// <summary>Const version of find() - Search Given element in the values array of Datum.</summary>
		/// <param name="t_item">Const reference to a RTTI class</param>
		/// <returns>Returns const std::tuple of boolean result & index of an element. Index will be size of an array if element isn't found.</returns>
		const std::tuple<bool, uint32_t> find(const RTTI& t_item) const;

		/// <summary>Const version of find() - Search Given element in the values array of Datum.</summary>
		/// <param name="t_item">Const reference to a Scope.</param>
		/// <returns>Returns const std::tuple of boolean result & index of an element. Index will be size of an array if element isn't found.</returns>
		const std::tuple<bool, uint32_t> find(const Scope& t_item) const;

		/// <summary>Nested Scope dereference operator for datum.</summary>
		/// <param name="t_index">index of a Scope to get from.</param>
		/// <returns>Returns reference to the Scope.</returns>
		Scope& operator[](uint32_t t_index);

		/// <summary>Const version of Nested Scope dereference operator for datum.</summary>
		/// <param name="t_index">index of a Scope to get from.</param>
		/// <returns>Returns const reference to the Scope.</returns>
		const Scope& operator[](uint32_t t_index) const;
	private:
		/// <summary>Discriminated Union for storing Data pointer.</summary>
		union Data
		{
			int32_t* d_int32_t = nullptr;
			float_t* d_float_t;
			glm::vec4* d_glm_vec4;
			glm::mat4x4* d_glm_mat4x4;
			std::string* d_string;
			RTTI** d_RTTI_ptr;
			Scope** d_scope_ptr;
			void* d_void_ptr;
		};

		Data m_data;
		DatumType m_type = DatumType::UNKNOWN;
		DatumStorageType m_storage_type = DatumStorageType::UNKNOWN;
		uint32_t m_size = 0;
		uint32_t m_capacity = 0;
		bool m_is_external = false;
		//static uint32_t typeSizeMap[];
		const static inline size_t typeSizeMap[8]{ 0, sizeof(int32_t), sizeof(float_t), sizeof(glm::vec4), sizeof(glm::mat4x4), sizeof(Scope*), sizeof(std::string), sizeof(RTTI*) };
	};
}