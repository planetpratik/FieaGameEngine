#include "pch.h"
#include "Datum.h"

namespace FieaGameEngine
{
	Datum::Datum() :
		m_type(DatumType::UNKNOWN), m_storage_type(DatumStorageType::UNKNOWN), m_size(0), m_capacity(0), m_is_external(false)
	{
	}

	Datum::Datum(const Datum& t_rhs) :
		m_type(DatumType::UNKNOWN), m_storage_type(DatumStorageType::UNKNOWN), m_size(0), m_capacity(0), m_is_external(false)
	{
		operator=(t_rhs);
	}

	Datum::Datum(Datum&& t_rhs):
		m_data(t_rhs.m_data), m_type(t_rhs.m_type), m_storage_type(t_rhs.m_storage_type), m_size(t_rhs.m_size), m_capacity(t_rhs.m_capacity), m_is_external(t_rhs.m_is_external)
	{
		t_rhs.m_size = 0;
		t_rhs.m_capacity = 0;
		t_rhs.m_is_external = false;
		t_rhs.m_storage_type = DatumStorageType::UNKNOWN;
		t_rhs.m_type = DatumType::UNKNOWN;
		t_rhs.m_data.d_int32_t = nullptr;
	}

	Datum& Datum::operator=(const Datum& t_rhs)
	{
		if (this != &t_rhs)
		{
			if (!m_is_external)
			{
				clear();
			}
			m_type = t_rhs.m_type;
			m_storage_type = t_rhs.m_storage_type;
			m_is_external = t_rhs.m_is_external;
			if (t_rhs.m_storage_type == DatumStorageType::INTERNAL)
			{
				reserve(t_rhs.m_capacity);
				for (uint32_t i = 0; i < t_rhs.m_size; ++i)
				{
					switch (m_type)
					{
					case DatumType::INTEGER:
						set(t_rhs.m_data.d_int32_t[i], i);
						break;
					case DatumType::FLOAT:
						set(t_rhs.m_data.d_float_t[i], i);
						break;
					case DatumType::VECTOR4:
						set(t_rhs.m_data.d_glm_vec4[i], i);
						break;
					case DatumType::MATRIX4X4:
						set(t_rhs.m_data.d_glm_mat4x4[i], i);
						break;
					case DatumType::STRING:
						set(t_rhs.m_data.d_string[i], i);
						break;
					case DatumType::POINTER:
						set(t_rhs.m_data.d_RTTI_ptr[i], i);
						break;
					default:
						break;
					}
				}
			}
			else
			{
				m_data = t_rhs.m_data;
				m_capacity = t_rhs.m_capacity;
			}
			m_size = t_rhs.m_size;
		}
		return *this;
	}

	Datum& Datum::operator=(Datum&& t_rhs)
	{
		if (this != &t_rhs)
		{
			clear();
			m_size = t_rhs.m_size;
			m_capacity = t_rhs.m_capacity;
			m_type = t_rhs.m_type;
			m_storage_type = t_rhs.m_storage_type;
			m_data = t_rhs.m_data;
			m_is_external = t_rhs.m_is_external;

			t_rhs.m_size = 0;
			t_rhs.m_capacity = 0;
			t_rhs.m_type = DatumType::UNKNOWN;
			t_rhs.m_storage_type = DatumStorageType::UNKNOWN;
			t_rhs.m_is_external = false;
		}
		return *this;
	}

	Datum& Datum::operator=(const int32_t& t_rhs)
	{
		set(t_rhs);
		return *this;
	}

	Datum& Datum::operator=(const float_t& t_rhs)
	{
		set(t_rhs);
		return *this;
	}

	Datum& Datum::operator=(const glm::vec4& t_rhs)
	{
		set(t_rhs);
		return *this;
	}

	Datum& Datum::operator=(const glm::mat4x4& t_rhs)
	{
		set(t_rhs);
		return *this;
	}

	Datum& Datum::operator=(const std::string& t_rhs)
	{
		set(t_rhs);
		return *this;
	}

	Datum& Datum::operator=(RTTI* t_rhs)
	{
		set(t_rhs);
		return *this;
	}

	Datum::~Datum()
	{
		clear();
	}

	Datum::DatumType Datum::type() const
	{
		return m_type;
	}

	void Datum::setType(DatumType t_type)
	{
		if (m_type != DatumType::UNKNOWN && m_type != t_type)
		{
			throw std::exception("Invalid Operation! Once assigned, Datum's Data Type cannot be changed.");
		}
		m_type = t_type;
		if (m_storage_type == DatumStorageType::UNKNOWN)
		{
			m_storage_type = DatumStorageType::INTERNAL;
		}
	}

	uint32_t Datum::size() const
	{
		return m_size;
	}

	uint32_t Datum::capacity() const
	{
		return m_capacity;
	}

	Datum::DatumStorageType Datum::storageType() const
	{
		return m_storage_type;
	}

	bool Datum::isExternal() const
	{
		return m_is_external;
	}

	void Datum::setSize(uint32_t t_size)
	{
		if (m_type == DatumType::UNKNOWN)
		{
			throw std::exception("Invalid Operation! Datum's type isn't valid.");
		}
		if (m_is_external)
		{
			throw std::exception("Invalid Operation! Cannot set size for Datum belonging to External Storage.");
		}
		if (m_capacity < t_size)
		{
			reserve(t_size);
		}
		while (m_size < t_size)
		{
			switch (m_type)
			{
			case DatumType::INTEGER:
				new (m_data.d_int32_t + m_size)int32_t();
				++m_size;
				break;
			case DatumType::FLOAT:
				new (m_data.d_float_t + m_size)float_t();
				++m_size;
				break;
			case DatumType::VECTOR4:
				new (m_data.d_glm_vec4 + m_size)glm::vec4();
				++m_size;
				break;
			case DatumType::MATRIX4X4:
				new (m_data.d_glm_mat4x4 + m_size)glm::mat4x4();
				++m_size;
				break;
			case DatumType::STRING:
				new (m_data.d_string + m_size)std::string();
				++m_size;
				break;
			case DatumType::POINTER:
				new (m_data.d_RTTI_ptr + m_size)RTTI*(nullptr);
				++m_size;
				break;
			default:
				throw std::exception("Invalid Operation! Data type not valid.");
			}
		}
		while (m_size > t_size)
		{
			switch (m_type)
			{
			case DatumType::INTEGER:
				--m_size;
				m_data.d_int32_t[m_size].std::int32_t::~int32_t();
				break;
			case DatumType::FLOAT:
				--m_size;
				m_data.d_float_t[m_size].std::float_t::~float_t();
				break;
			case DatumType::VECTOR4:
				--m_size;
				m_data.d_glm_vec4[m_size].glm::vec4::~vec4();
				break;
			case DatumType::MATRIX4X4:
				--m_size;
				m_data.d_glm_mat4x4[m_size].glm::mat4x4::~mat4x4();
				break;
			case DatumType::STRING:
				--m_size;
				m_data.d_string[m_size].std::string::~string();
				break;
			case DatumType::POINTER:
				--m_size;
				new (m_data.d_RTTI_ptr + m_size)RTTI*(nullptr);
				break;
			default:
				throw std::exception("Invalid Operation! Data type not valid.");
			}
		}
	}

	void Datum::reserve(uint32_t t_new_capacity)
	{
		if (m_is_external || m_storage_type == DatumStorageType::UNKNOWN)
		{
			throw std::exception("Invalid Operation! Memory reservation isn't possible for Unknown / External storage.");
		}
		if (m_capacity < t_new_capacity)
		{
			switch (m_type)
			{
			case DatumType::INTEGER:
				m_data.d_int32_t = reinterpret_cast<int32_t*>(realloc(m_data.d_int32_t, t_new_capacity * sizeof(int32_t)));
				break;
			case DatumType::FLOAT:
				m_data.d_float_t = reinterpret_cast<float_t*>(realloc(m_data.d_float_t, t_new_capacity * sizeof(float_t)));
				break;
			case DatumType::VECTOR4:
				m_data.d_glm_vec4 = reinterpret_cast<glm::vec4*>(realloc(m_data.d_glm_vec4, t_new_capacity * sizeof(glm::vec4)));
				break;
			case DatumType::MATRIX4X4:
				m_data.d_glm_mat4x4 = reinterpret_cast<glm::mat4x4*>(realloc(m_data.d_glm_mat4x4, t_new_capacity * sizeof(glm::mat4x4)));
				break;
			case DatumType::STRING:
				m_data.d_string = reinterpret_cast<std::string*>(realloc(m_data.d_string, t_new_capacity * sizeof(std::string)));
				break;
			case DatumType::POINTER:
				m_data.d_RTTI_ptr = reinterpret_cast<RTTI**>(realloc(m_data.d_glm_vec4, t_new_capacity * sizeof(RTTI*)));
				break;
			default:
				throw std::exception("Invalid Operation! Data type is not valid.");
			}
			m_capacity = t_new_capacity;
		}
	}

	void Datum::clear()
	{
		if (m_storage_type == DatumStorageType::INTERNAL && m_capacity != 0)
		{
			switch (m_type)
			{
			case DatumType::INTEGER:
				free(m_data.d_int32_t);
				break;
			case DatumType::FLOAT:
				free(m_data.d_float_t);
				break;
			case DatumType::VECTOR4:
				for (uint32_t i = 0; i < m_size; ++i)
				{
					m_data.d_glm_vec4[i].glm::vec4::~vec4();
				}
				free(m_data.d_glm_vec4);
				break;
			case DatumType::MATRIX4X4:
				for (uint32_t i = 0; i < m_size; ++i)
				{
					m_data.d_glm_mat4x4[i].glm::mat4x4::~mat4x4();
				}
				free(m_data.d_glm_mat4x4);
				break;
			case DatumType::STRING:
				for (uint32_t i = 0; i < m_size; ++i)
				{
					m_data.d_string[i].std::string::~basic_string();
				}
				free(m_data.d_string);
				break;
			case DatumType::POINTER:
				free(m_data.d_RTTI_ptr);
				break;
			default:
				break;
			}
			m_size = 0;
			m_capacity = 0;
			m_type = DatumType::UNKNOWN;
			m_storage_type = DatumStorageType::UNKNOWN;
			m_is_external = false;
		}
	}

	void Datum::setStorage(const DatumType& t_type, uint32_t t_array_size)
	{
		if (m_type != DatumType::UNKNOWN && m_type != t_type)
		{
			throw std::exception("Invalid operation! Datum types are different.");
		}
		if (!m_is_external)
		{
			clear();
		}
		m_type = t_type;
		m_storage_type = DatumStorageType::EXTERNAL;
		m_size = t_array_size;
		m_capacity = t_array_size;
		m_is_external = true;
	}

	void Datum::setStorage(int32_t* t_array, uint32_t t_array_size)
	{
		setStorage(DatumType::INTEGER, t_array_size);
		m_data.d_int32_t = t_array;
	}

	void Datum::setStorage(float_t* t_array, uint32_t t_array_size)
	{
		setStorage(DatumType::FLOAT, t_array_size);
		m_data.d_float_t = t_array;
	}

	void Datum::setStorage(glm::vec4* t_array, uint32_t t_array_size)
	{
		setStorage(DatumType::VECTOR4, t_array_size);
		m_data.d_glm_vec4 = t_array;
	}

	void Datum::setStorage(glm::mat4x4* t_array, uint32_t t_array_size)
	{
		setStorage(DatumType::MATRIX4X4, t_array_size);
		m_data.d_glm_mat4x4 = t_array;
	}

	void Datum::setStorage(std::string* t_array, uint32_t t_array_size)
	{
		setStorage(DatumType::STRING, t_array_size);
		m_data.d_string = t_array;
	}

	void Datum::setStorage(RTTI** t_array, uint32_t t_array_size)
	{
		setStorage(DatumType::POINTER, t_array_size);
		m_data.d_RTTI_ptr = t_array;
	}

	bool Datum::operator==(const Datum& t_rhs) const
	{
		bool are_equal = false;
		if ((m_type == t_rhs.m_type) && (m_size == t_rhs.m_size))
		{
			are_equal = true;
			for (uint32_t i = 0; i < m_size; ++i)
			{
				switch (m_type)
				{
				case DatumType::INTEGER:
					if (m_data.d_int32_t[i] != t_rhs.m_data.d_int32_t[i])
					{
						are_equal = false;
						break;
					}
					break;
				case DatumType::FLOAT:
					if (m_data.d_float_t[i] != t_rhs.m_data.d_float_t[i])
					{
						are_equal = false;
						break;
					}
					break;
				case DatumType::VECTOR4:
					if (m_data.d_glm_vec4[i] != t_rhs.m_data.d_glm_vec4[i])
					{
						are_equal = false;
						break;
					}
					break;
				case DatumType::MATRIX4X4:
					if (m_data.d_glm_mat4x4[i] != t_rhs.m_data.d_glm_mat4x4[i])
					{
						are_equal = false;
						break;
					}
					break;
				case DatumType::STRING:
					if (m_data.d_string[i] != t_rhs.m_data.d_string[i])
					{
						are_equal = false;
						break;
					}
					break;
				case DatumType::POINTER:
					if (m_data.d_RTTI_ptr[i] != t_rhs.m_data.d_RTTI_ptr[i])
					{
						are_equal = false;
						break;
					}
					break;
				default:
					throw std::exception("Invalid Operation! Data type is not valid.");
				}
			}
		}
		return are_equal;
	}

	bool Datum::operator==(const int32_t& t_rhs) const
	{
		return ((m_type == DatumType::INTEGER)&&(m_size > 0)&&(*m_data.d_int32_t == t_rhs));
	}

	bool Datum::operator==(const float_t& t_rhs) const
	{
		return ((m_type == DatumType::FLOAT) && (m_size > 0) && (*m_data.d_float_t == t_rhs));
	}

	bool Datum::operator==(const glm::vec4& t_rhs) const
	{
		return ((m_type == DatumType::VECTOR4) && (m_size > 0) && (*m_data.d_glm_vec4 == t_rhs));
	}

	bool Datum::operator==(const glm::mat4x4& t_rhs) const
	{
		return ((m_type == DatumType::MATRIX4X4) && (m_size > 0) && (*m_data.d_glm_mat4x4 == t_rhs));
	}

	bool Datum::operator==(const std::string& t_rhs) const
	{
		return ((m_type == DatumType::STRING) && (m_size > 0) && (t_rhs.compare(*m_data.d_string) == 0));
	}

	bool Datum::operator==(const RTTI* t_rhs) const
	{
		return ((m_type == DatumType::POINTER) && (m_size > 0) && ((**m_data.d_RTTI_ptr).Equals(t_rhs)));
	}

	bool Datum::operator!=(const Datum& t_rhs) const
	{
		return !(operator==(t_rhs));
	}

	bool Datum::operator!=(const int32_t& t_rhs) const
	{
		return !(operator==(t_rhs));
	}

	bool Datum::operator!=(const float_t& t_rhs) const
	{
		return !(operator==(t_rhs));
	}

	bool Datum::operator!=(const glm::vec4& t_rhs) const
	{
		return !(operator==(t_rhs));
	}

	bool Datum::operator!=(const glm::mat4x4& t_rhs) const
	{
		return !(operator==(t_rhs));
	}

	bool Datum::operator!=(const std::string& t_rhs) const
	{
		return !(operator==(t_rhs));
	}

	bool Datum::operator!=(const RTTI* t_rhs) const
	{
		return !(operator==(t_rhs));
	}

	void Datum::set(const DatumType& t_type, uint32_t t_index)
	{
		if (m_type == DatumType::UNKNOWN)
		{
			m_type = t_type;
			m_storage_type = DatumStorageType::INTERNAL;
			m_is_external = false;
		}
		if (m_type != t_type)
		{
			throw std::exception("Invalid Operation! Cannot perform set due to type mismatch.");
		}
		if (t_index >= m_size)
		{
			if (m_storage_type == DatumStorageType::EXTERNAL)
			{
				throw std::exception("Invalid Operation! External storage cannot be increased hence set operation failed.");
			}
			setSize(t_index + 1);
		}
	}

	void Datum::set(const int32_t& t_value, uint32_t t_index)
	{
		set(DatumType::INTEGER, t_index);
		m_data.d_int32_t[t_index] = t_value;
	}

	void Datum::set(const float_t& t_value, uint32_t t_index)
	{
		set(DatumType::FLOAT, t_index);
		m_data.d_float_t[t_index] = t_value;
	}
	
	void Datum::set(const glm::vec4& t_value, uint32_t t_index)
	{
		set(DatumType::VECTOR4, t_index);
		m_data.d_glm_vec4[t_index] = t_value;
	}

	void Datum::set(const glm::mat4x4& t_value, uint32_t t_index)
	{
		set(DatumType::MATRIX4X4, t_index);
		m_data.d_glm_mat4x4[t_index] = t_value;
	}

	void Datum::set(const std::string& t_value, uint32_t t_index)
	{
		set(DatumType::STRING, t_index);
		m_data.d_string[t_index] = t_value;
	}

	void Datum::set(RTTI* t_value, uint32_t t_index)
	{
		set(DatumType::POINTER, t_index);
		m_data.d_RTTI_ptr[t_index] = t_value;
	}

	template<>
	int32_t& Datum::get(uint32_t t_index)
	{
		if (m_type != DatumType::INTEGER)
		{
			throw std::exception("Invalid Operation! Cannot get an Integer due to type mismatch");
		}
		if (m_size <= t_index)
		{
			throw std::exception("Invalid Operation! Index out of bounds.");
		}
		return m_data.d_int32_t[t_index];
	}

	template<>
	float_t& Datum::get(uint32_t t_index)
	{
		if (m_type != DatumType::FLOAT)
		{
			throw std::exception("Invalid Operation! Cannot get a Float due to type mismatch");
		}
		if (m_size <= t_index)
		{
			throw std::exception("Invalid Operation! Index out of bounds.");
		}
		return m_data.d_float_t[t_index];
	}

	template<>
	glm::vec4& Datum::get(uint32_t t_index)
	{
		if (m_type != DatumType::VECTOR4)
		{
			throw std::exception("Invalid Operation! Cannot get a GLM Vector due to type mismatch");
		}
		if (m_size <= t_index)
		{
			throw std::exception("Invalid Operation! Index out of bounds.");
		}
		return m_data.d_glm_vec4[t_index];
	}

	template<>
	glm::mat4x4& Datum::get(uint32_t t_index)
	{
		if (m_type != DatumType::MATRIX4X4)
		{
			throw std::exception("Invalid Operation! Cannot get a GLM Matrix due to type mismatch");
		}
		if (m_size <= t_index)
		{
			throw std::exception("Invalid Operation! Index out of bounds.");
		}
		return m_data.d_glm_mat4x4[t_index];
	}

	template<>
	std::string& Datum::get(uint32_t t_index)
	{
		if (m_type != DatumType::STRING)
		{
			throw std::exception("Invalid Operation! Cannot get a String due to type mismatch");
		}
		if (m_size <= t_index)
		{
			throw std::exception("Invalid Operation! Index out of bounds.");
		}
		return m_data.d_string[t_index];
	}

	template<>
	RTTI*& Datum::get(uint32_t t_index)
	{
		if (m_type != DatumType::POINTER)
		{
			throw std::exception("Invalid Operation! Cannot get a RTTI Pointer due to type mismatch");
		}
		if (m_size <= t_index)
		{
			throw std::exception("Invalid Operation! Index out of bounds.");
		}
		return m_data.d_RTTI_ptr[t_index];
	}

	template<>
	const int32_t& Datum::get(uint32_t t_index) const
	{
		if (m_type != DatumType::INTEGER)
		{
			throw std::exception("Invalid Operation! Cannot get an Integer due to type mismatch");
		}
		if (m_size <= t_index)
		{
			throw std::exception("Invalid Operation! Index out of bounds.");
		}
		return m_data.d_int32_t[t_index];
	}

	template<>
	const float_t& Datum::get(uint32_t t_index) const
	{
		if (m_type != DatumType::FLOAT)
		{
			throw std::exception("Invalid Operation! Cannot get a Float due to type mismatch");
		}
		if (m_size <= t_index)
		{
			throw std::exception("Invalid Operation! Index out of bounds.");
		}
		return m_data.d_float_t[t_index];
	}

	template<>
	const glm::vec4& Datum::get(uint32_t t_index) const
	{
		if (m_type != DatumType::VECTOR4)
		{
			throw std::exception("Invalid Operation! Cannot get a GLM Vector due to type mismatch");
		}
		if (m_size <= t_index)
		{
			throw std::exception("Invalid Operation! Index out of bounds.");
		}
		return m_data.d_glm_vec4[t_index];
	}

	template<>
	const glm::mat4x4& Datum::get(uint32_t t_index) const
	{
		if (m_type != DatumType::MATRIX4X4)
		{
			throw std::exception("Invalid Operation! Cannot get a GLM Matrix due to type mismatch");
		}
		if (m_size <= t_index)
		{
			throw std::exception("Invalid Operation! Index out of bounds.");
		}
		return m_data.d_glm_mat4x4[t_index];
	}

	template<>
	const std::string& Datum::get(uint32_t t_index) const
	{
		if (m_type != DatumType::STRING)
		{
			throw std::exception("Invalid Operation! Cannot get a String due to type mismatch");
		}
		if (m_size <= t_index)
		{
			throw std::exception("Invalid Operation! Index out of bounds.");
		}
		return m_data.d_string[t_index];
	}

	template<>
	RTTI* const& Datum::get(uint32_t t_index) const
	{
		if (m_type != DatumType::POINTER)
		{
			throw std::exception("Invalid Operation! Cannot get a RTTI Pointer due to type mismatch");
		}
		if (m_size <= t_index)
		{
			throw std::exception("Invalid Operation! Index out of bounds.");
		}
		return m_data.d_RTTI_ptr[t_index];
	}

	void Datum::setFromString(const std::string& t_input_string, uint32_t t_index)
	{
		if (m_type == DatumType::UNKNOWN)
		{
			throw std::exception("Invalid Operation! Datum Type isn't set.");
		}
		std::stringstream input_stream(t_input_string);
		switch (m_type)
		{
		case DatumType::INTEGER:
			int32_t temp_int;
			input_stream >> temp_int;
			set(temp_int,t_index);
			break;
		case DatumType::FLOAT:
			float_t temp_float;
			input_stream >> temp_float;
			set(temp_float, t_index);
			break;
		case DatumType::VECTOR4:
			glm::vec4 temp_vector;
			sscanf_s(t_input_string.c_str(), "vec4(%f, %f, %f, %f)", &temp_vector.x, &temp_vector.y, &temp_vector.z, &temp_vector.w);
			set(temp_vector, t_index);
			break;
		case DatumType::MATRIX4X4:
			glm::mat4x4 temp_mat;
			sscanf_s(t_input_string.c_str(), "mat4x4((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f))",
				&temp_mat[0][0], &temp_mat[0][1], &temp_mat[0][2], &temp_mat[0][3],
				&temp_mat[1][0], &temp_mat[1][1], &temp_mat[1][2], &temp_mat[1][3],
				&temp_mat[2][0], &temp_mat[2][1], &temp_mat[2][2], &temp_mat[2][3],
				&temp_mat[3][0], &temp_mat[3][1], &temp_mat[3][2], &temp_mat[3][3]);
			set(temp_mat, t_index);
			break;
		case DatumType::STRING:
			set(t_input_string, t_index);
			break;
		case DatumType::POINTER:
			throw std::exception("Invalid Operation! Cannot set pointer from string input.");
			break;
		default:
			throw std::exception("Invalid Operation! Data type is not valid.");
		}
	}

	std::string Datum::toString(uint32_t t_index)
	{
		if (m_type == DatumType::UNKNOWN)
		{
			throw std::exception("Invalid Operation! Datum Type isn't set.");
		}
		std::string output_string;
		switch (m_type)
		{
		case DatumType::INTEGER:
			output_string = std::to_string(get<int32_t>(t_index));
			break;
		case DatumType::FLOAT:
			output_string = std::to_string(get<float_t>(t_index));
			break;
		case DatumType::VECTOR4:
			output_string = glm::to_string(get<glm::vec4>(t_index));
			break;
		case DatumType::MATRIX4X4:
			output_string = glm::to_string(get<glm::mat4x4>(t_index));
			break;
		case DatumType::STRING:
			output_string = get<std::string>(t_index);
			break;
		case DatumType::POINTER:
			if (m_data.d_RTTI_ptr[t_index] != nullptr)
			{
				output_string = get<RTTI*>(t_index)->ToString();
			}
			break;
		default:
			throw std::exception("Invalid Operation! Data type is not valid.");
		}
		return output_string;
	}

	void Datum::pushBack(const int32_t& t_item)
	{
		if (m_is_external)
		{
			throw std::exception("Invalid Operation! Cannot write data to external storage.");
		}
		if (m_type == DatumType::UNKNOWN)
		{
			m_type = DatumType::INTEGER;
			m_storage_type = DatumStorageType::INTERNAL;
		}
		if (m_type != DatumType::INTEGER)
		{
			throw std::exception("Invalid Operation! Datum type must be an integer.");
		}
		if (m_size == m_capacity)
		{
			reserve(m_capacity + 3);
		}
		m_data.d_int32_t[m_size] = t_item;
		++m_size;
	}

	void Datum::pushBack(const float_t& t_item)
	{
		if (m_is_external)
		{
			throw std::exception("Invalid Operation! Cannot write data to external storage.");
		}
		if (m_type == DatumType::UNKNOWN)
		{
			m_type = DatumType::FLOAT;
			m_storage_type = DatumStorageType::INTERNAL;
		}
		if (m_type != DatumType::FLOAT)
		{
			throw std::exception("Invalid Operation! Datum type must be a float.");
		}
		if (m_size == m_capacity)
		{
			reserve(m_capacity + 3);
		}
		m_data.d_float_t[m_size] = t_item;
		++m_size;
	}

	void Datum::pushBack(const glm::vec4& t_item)
	{
		if (m_is_external)
		{
			throw std::exception("Invalid Operation! Cannot write data to external storage.");
		}
		if (m_type == DatumType::UNKNOWN)
		{
			m_type = DatumType::VECTOR4;
			m_storage_type = DatumStorageType::INTERNAL;
		}
		if (m_type != DatumType::VECTOR4)
		{
			throw std::exception("Invalid Operation! Datum type must be a glm Vector.");
		}
		if (m_size == m_capacity)
		{
			reserve(m_capacity + 3);
		}
		m_data.d_glm_vec4[m_size] = t_item;
		++m_size;
	}

	void Datum::pushBack(const glm::mat4x4& t_item)
	{
		if (m_is_external)
		{
			throw std::exception("Invalid Operation! Cannot write data to external storage.");
		}
		if (m_type == DatumType::UNKNOWN)
		{
			m_type = DatumType::MATRIX4X4;
			m_storage_type = DatumStorageType::INTERNAL;
		}
		if (m_type != DatumType::MATRIX4X4)
		{
			throw std::exception("Invalid Operation! Datum type must be a glm Matrix.");
		}
		if (m_size == m_capacity)
		{
			reserve(m_capacity + 3);
		}
		m_data.d_glm_mat4x4[m_size] = t_item;
		++m_size;
	}

	void Datum::pushBack(const std::string& t_item)
	{
		if (m_is_external)
		{
			throw std::exception("Invalid Operation! Cannot write data to external storage.");
		}
		if (m_type == DatumType::UNKNOWN)
		{
			m_type = DatumType::STRING;
			m_storage_type = DatumStorageType::INTERNAL;
		}
		if (m_type != DatumType::STRING)
		{
			throw std::exception("Invalid Operation! Datum type must be a String.");
		}
		if (m_size == m_capacity)
		{
			reserve(m_capacity + 3);
		}
		new (m_data.d_string + m_size) std::string(t_item);
		++m_size;
	}

	void Datum::pushBack(RTTI* t_item)
	{
		if (m_is_external)
		{
			throw std::exception("Invalid Operation! Cannot write data to external storage.");
		}
		if (m_type == DatumType::UNKNOWN)
		{
			m_type = DatumType::POINTER;
			m_storage_type = DatumStorageType::INTERNAL;
		}
		if (m_type != DatumType::POINTER)
		{
			throw std::exception("Invalid Operation! Datum type must be a RTTI pointer.");
		}
		if (m_size == m_capacity)
		{
			reserve(m_capacity + 3);
		}
		m_data.d_RTTI_ptr[m_size] = t_item;
		++m_size;
	}

	void Datum::popBack()
	{
		if (m_is_external)
		{
			throw std::exception("Invalid Operation! Cannot delete data from external storage.");
		}
		if (m_type == DatumType::UNKNOWN)
		{
			throw std::exception("Invalid Operation! Cannot perform popback on Datum with Unknown type.");
		}
		if (m_size == 0)
		{
			throw std::exception("Invalid Operation! Cannot perform popBack on empty Datum.");
		}
		if (m_size > 0)
		{
			--m_size;
			switch (m_type)
			{
			case DatumType::INTEGER:
				break;
			case DatumType::FLOAT:
				break;
			case DatumType::VECTOR4:
				m_data.d_glm_vec4[m_size].glm::vec4::~vec4();
				break;
			case DatumType::MATRIX4X4:
				m_data.d_glm_mat4x4[m_size].glm::mat4x4::~mat4x4();
				break;
			case DatumType::STRING:
				m_data.d_string[m_size].std::string::~string();
				break;
			case DatumType::POINTER:
				m_data.d_RTTI_ptr[m_size] = nullptr;
				break;
			default:
				++m_size;
				throw std::exception("Invalid Operation! Data type is not valid.");
			}
		}
	}

	template<>
	int32_t& Datum::front<int32_t>(uint32_t /*placeholder*/)
	{
		if (m_size > 0)
		{
			return get<int32_t>(0);
		}
		throw std::exception("Invalid Operation! Cannot perform front operation on empty array.");
	}

	template<>
	float_t& Datum::front(uint32_t /*placeholder*/)
	{
		if (m_size > 0)
		{
			return get<float_t>(0);
		}
		throw std::exception("Invalid Operation! Cannot perform front operation on empty array.");
	}

	template<>
	glm::vec4& Datum::front(uint32_t /*placeholder*/)
	{
		if (m_size > 0)
		{
			return get<glm::vec4>(0);
		}
		throw std::exception("Invalid Operation! Cannot perform front operation on empty array.");
	}

	template<>
	glm::mat4x4& Datum::front(uint32_t /*placeholder*/)
	{
		if (m_size > 0)
		{
			return get<glm::mat4x4>(0);
		}	
		throw std::exception("Invalid Operation! Cannot perform front operation on empty array.");
	}

	template<>
	std::string& Datum::front(uint32_t /*placeholder*/)
	{
		if (m_size > 0)
		{
			return get<std::string>(0);
		}
		throw std::exception("Invalid Operation! Cannot perform front operation on empty array.");
	}

	template<>
	RTTI*& Datum::front(uint32_t /*placeholder*/)
	{
		if (m_size > 0)
		{
			return get<RTTI*>(0);
		}		
		throw std::exception("Invalid Operation! Cannot perform front operation on empty array.");
	}

	template<>
	const int32_t& Datum::front(uint32_t /*placeholder*/) const
	{
		if (m_size > 0)
		{
			return Datum::get<int32_t>(0);
		}
		throw std::exception("Invalid Operation! Cannot perform front operation on empty array.");
	}

	template<>
	const float_t& Datum::front(uint32_t /*placeholder*/) const
	{
		if (m_size > 0)
		{
			return get<float_t>(0);
		}
		throw std::exception("Invalid Operation! Cannot perform front operation on empty array.");
	}

	template<>
	const glm::vec4& Datum::front(uint32_t /*placeholder*/) const
	{
		if (m_size > 0)
		{
			return get<glm::vec4>(0);
		}
		throw std::exception("Invalid Operation! Cannot perform front operation on empty array.");
	}

	template<>
	const glm::mat4x4& Datum::front(uint32_t /*placeholder*/) const
	{
		if (m_size > 0)
		{
			return get<glm::mat4x4>(0);
		}
		throw std::exception("Invalid Operation! Cannot perform front operation on empty array.");
	}

	template<>
	const std::string& Datum::front(uint32_t /*placeholder*/) const
	{
		if (m_size > 0)
		{
			return get<std::string>(0);
		}
		throw std::exception("Invalid Operation! Cannot perform front operation on empty array.");
	}

	template<>
	RTTI* const& Datum::front(uint32_t /*placeholder*/) const
	{
		if (m_size > 0)
		{
			return get<RTTI*>(0);
		}
		throw std::exception("Invalid Operation! Cannot perform front operation on empty array.");
	}

	template<>
	int32_t& Datum::back(uint32_t /*placeholder*/)
	{
		if (m_size > 0)
		{
			return get<int32_t>(m_size - 1);
		}
		throw std::exception("Invalid Operation! Cannot perform back operation on empty array.");
	}

	template<>
	float_t& Datum::back(uint32_t /*placeholder*/)
	{
		if (m_size > 0)
		{
			return get<float_t>(m_size - 1);
		}
		throw std::exception("Invalid Operation! Cannot perform back operation on empty array.");
	}

	template<>
	glm::vec4& Datum::back(uint32_t /*placeholder*/)
	{
		if (m_size > 0)
		{
			return get<glm::vec4>(m_size - 1);
		}
		throw std::exception("Invalid Operation! Cannot perform back operation on empty array.");
	}

	template<>
	glm::mat4x4& Datum::back(uint32_t /*placeholder*/)
	{
		if (m_size > 0)
		{
			return get<glm::mat4x4>(m_size - 1);
		}
		throw std::exception("Invalid Operation! Cannot perform back operation on empty array.");
	}

	template<>
	std::string& Datum::back(uint32_t /*placeholder*/)
	{
		if (m_size > 0)
		{
			return get<std::string>(m_size - 1);
		}
		throw std::exception("Invalid Operation! Cannot perform back operation on empty array.");
	}

	template<>
	RTTI*& Datum::back(uint32_t /*placeholder*/)
	{
		if (m_size > 0)
		{
			return get<RTTI*>(m_size - 1);
		}
		throw std::exception("Invalid Operation! Cannot perform back operation on empty array.");
	}

	template<>
	const int32_t& Datum::back(uint32_t /*placeholder*/) const
	{
		if (m_size > 0)
		{
			return get<int32_t>(m_size - 1);
		}
		throw std::exception("Invalid Operation! Cannot perform back operation on empty array.");
	}

	template<>
	const float_t& Datum::back(uint32_t /*placeholder*/) const
	{
		if (m_size > 0)
		{
			return get<float_t>(m_size - 1);
		}
		throw std::exception("Invalid Operation! Cannot perform back operation on empty array.");
	}

	template<>
	const glm::vec4& Datum::back(uint32_t /*placeholder*/) const
	{
		if (m_size > 0)
		{
			return get<glm::vec4>(m_size - 1);
		}
		throw std::exception("Invalid Operation! Cannot perform back operation on empty array.");
	}

	template<>
	const glm::mat4x4& Datum::back(uint32_t /*placeholder*/) const
	{
		if (m_size > 0)
		{
			return get<glm::mat4x4>(m_size - 1);
		}
		throw std::exception("Invalid Operation! Cannot perform back operation on empty array.");
	}

	template<>
	const std::string& Datum::back(uint32_t /*placeholder*/) const
	{
		if (m_size > 0)
		{
			return get<std::string>(m_size - 1);
		}
		throw std::exception("Invalid Operation! Cannot perform back operation on empty array.");
	}

	template<>
	RTTI* const& Datum::back(uint32_t /*placeholder*/) const
	{
		if (m_size > 0)
		{
			return get<RTTI*>(m_size - 1);
		}
		throw std::exception("Invalid Operation! Cannot perform back operation on empty array.");
	}

	void Datum::removeAt(uint32_t t_index)
	{
		if (m_is_external)
		{
			throw std::exception("Invalid Operation! Cannot delete data from external storage.");
		}
		if (m_type == DatumType::UNKNOWN)
		{
			throw std::exception("Invalid Operation! Datum Type isn't set.");
		}
		if (t_index < m_size)
		{
			uint32_t i = t_index;
			switch (m_type)
			{
			case DatumType::INTEGER:
				for (; i < m_size - 1; ++i)
				{
					m_data.d_int32_t[i] = m_data.d_int32_t[i + 1];
				}
				break;
			case DatumType::FLOAT:
				for (; i < m_size - 1; ++i)
				{
					m_data.d_float_t[i] = m_data.d_float_t[i + 1];
				}
				break;
			case DatumType::VECTOR4:
				for (; i < m_size - 1; ++i)
				{
					m_data.d_glm_vec4[i] = m_data.d_glm_vec4[i + 1];
				}
				break;
			case DatumType::MATRIX4X4:
				for (; i < m_size - 1; ++i)
				{
					m_data.d_glm_mat4x4[i] = m_data.d_glm_mat4x4[i + 1];
				}
				break;
			case DatumType::STRING:
				for (; i < m_size - 1; ++i)
				{
					m_data.d_string[i] = m_data.d_string[i + 1];
				}
				m_data.d_string[m_size - 1].~basic_string();
				break;
			case DatumType::POINTER:
				for (; i < m_size - 1; ++i)
				{
					m_data.d_RTTI_ptr[i] = m_data.d_RTTI_ptr[i + 1];
				}
				m_data.d_RTTI_ptr[m_size - 1] = nullptr;
				break;
			default:
				throw std::exception("Invalid Operation! Data type is not valid.");
			}
			--m_size;
		}
	}

	void Datum::remove(const int32_t& t_item)
	{
		if (m_is_external)
		{
			throw std::exception("Invalid Operation! Cannot delete data from external storage.");
		}
		if (m_type == DatumType::UNKNOWN)
		{
			throw std::exception("Invalid Operation! Datum Type isn't set.");
		}
		uint32_t index = find(t_item);
		if (index < m_size)
		{
			for (; index < m_size - 1; ++index)
			{
				m_data.d_int32_t[index] = m_data.d_int32_t[index + 1];
			}
			--m_size;
		}
	}

	void Datum::remove(const float_t& t_item)
	{
		if (m_is_external)
		{
			throw std::exception("Invalid Operation! Cannot delete data from external storage.");
		}
		if (m_type == DatumType::UNKNOWN)
		{
			throw std::exception("Invalid Operation! Datum Type isn't set.");
		}
		uint32_t index = find(t_item);
		if (index < m_size)
		{
			for (; index < m_size - 1; ++index)
			{
				m_data.d_float_t[index] = m_data.d_float_t[index + 1];
			}
			--m_size;
		}
	}

	void Datum::remove(const glm::vec4& t_item)
	{
		if (m_is_external)
		{
			throw std::exception("Invalid Operation! Cannot delete data from external storage.");
		}
		if (m_type == DatumType::UNKNOWN)
		{
			throw std::exception("Invalid Operation! Datum Type isn't set.");
		}
		uint32_t index = find(t_item);
		if (index < m_size)
		{
			for (; index < m_size - 1; ++index)
			{
				m_data.d_glm_vec4[index] = m_data.d_glm_vec4[index + 1];
			}
			--m_size;
		}
	}

	void Datum::remove(const glm::mat4x4& t_item)
	{
		if (m_is_external)
		{
			throw std::exception("Invalid Operation! Cannot delete data from external storage.");
		}
		if (m_type == DatumType::UNKNOWN)
		{
			throw std::exception("Invalid Operation! Datum Type isn't set.");
		}
		uint32_t index = find(t_item);
		if (index < m_size)
		{
			for (; index < m_size - 1; ++index)
			{
				m_data.d_glm_mat4x4[index] = m_data.d_glm_mat4x4[index + 1];
			}
			--m_size;
		}
	}

	void Datum::remove(const std::string& t_item)
	{
		if (m_is_external)
		{
			throw std::exception("Invalid Operation! Cannot delete data from external storage.");
		}
		if (m_type == DatumType::UNKNOWN)
		{
			throw std::exception("Invalid Operation! Datum Type isn't set.");
		}
		uint32_t index = find(t_item);
		if (index < m_size)
		{
			for (; index < m_size - 1; ++index)
			{
				m_data.d_string[index] = m_data.d_string[index + 1];
			}
			m_data.d_string[m_size - 1].~basic_string();
			--m_size;
		}
	}

	void Datum::remove(const RTTI& t_item)
	{
		if (m_is_external)
		{
			throw std::exception("Invalid Operation! Cannot delete data from external storage.");
		}
		if (m_type == DatumType::UNKNOWN)
		{
			throw std::exception("Invalid Operation! Datum Type isn't set.");
		}
		uint32_t index = find(t_item);
		if (index < m_size)
		{
			for (; index < m_size - 1; ++index)
			{
				m_data.d_RTTI_ptr[index] = m_data.d_RTTI_ptr[index + 1];
			}
			--m_size;
		}
	}

	uint32_t& Datum::find(const int32_t& t_item)
	{
		bool found = false; 
		uint32_t index = 0;
		if (m_type == DatumType::UNKNOWN)
		{
			throw std::exception("Invalid Operation! Datum Type isn't set.");
		}
		if (m_type != DatumType::INTEGER)
		{
			throw std::exception("Invalid Operation! Find operation cannot be performed due to type mismatch.");
		}
		for (uint32_t i = 0; i < m_size; ++i)
		{
			if (t_item == m_data.d_int32_t[i])
			{
				found = true;
				index = i;
				break;
			}
		}
		return (found ? index : m_size);
	}

	uint32_t& Datum::find(const float_t& t_item)
	{
		bool found = false;
		uint32_t index = 0;
		if (m_type == DatumType::UNKNOWN)
		{
			throw std::exception("Invalid Operation! Datum Type isn't set.");
		}
		if (m_type != DatumType::FLOAT)
		{
			throw std::exception("Invalid Operation! Find operation cannot be performed due to type mismatch.");
		}
		for (uint32_t i = 0; i < m_size; ++i)
		{
			if (t_item == m_data.d_float_t[i])
			{
				found = true;
				index = i;
				break;
			}
		}
		return (found ? index : m_size);
	}

	uint32_t& Datum::find(const glm::vec4& t_item)
	{
		bool found = false;
		uint32_t index = 0;
		if (m_type == DatumType::UNKNOWN)
		{
			throw std::exception("Invalid Operation! Datum Type isn't set.");
		}
		if (m_type != DatumType::VECTOR4)
		{
			throw std::exception("Invalid Operation! Find operation cannot be performed due to type mismatch.");
		}
		for (uint32_t i = 0; i < m_size; ++i)
		{
			if (t_item == m_data.d_glm_vec4[i])
			{
				found = true;
				index = i;
				break;
			}
		}
		return (found ? index : m_size);
	}

	uint32_t& Datum::find(const glm::mat4x4& t_item)
	{
		bool found = false;
		uint32_t index = 0;
		if (m_type == DatumType::UNKNOWN)
		{
			throw std::exception("Invalid Operation! Datum Type isn't set.");
		}
		if (m_type != DatumType::MATRIX4X4)
		{
			throw std::exception("Invalid Operation! Find operation cannot be performed due to type mismatch.");
		}
		for (uint32_t i = 0; i < m_size; ++i)
		{
			if (t_item == m_data.d_glm_mat4x4[i])
			{
				found = true;
				index = i;
				break;
			}
		}
		return (found ? index : m_size);
	}

	uint32_t& Datum::find(const std::string& t_item)
	{
		bool found = false;
		uint32_t index = 0;
		if (m_type == DatumType::UNKNOWN)
		{
			throw std::exception("Invalid Operation! Datum Type isn't set.");
		}
		if (m_type != DatumType::STRING)
		{
			throw std::exception("Invalid Operation! Find operation cannot be performed due to type mismatch.");
		}
		for (uint32_t i = 0; i < m_size; ++i)
		{
			if (t_item == m_data.d_string[i])
			{
				found = true;
				index = i;
				break;
			}
		}
		return (found ? index : m_size);
	}

	uint32_t& Datum::find(const RTTI& t_item)
	{
		bool found = false;
		uint32_t index = 0;
		if (m_type == DatumType::UNKNOWN)
		{
			throw std::exception("Invalid Operation! Datum Type isn't set.");
		}
		if (m_type != DatumType::POINTER)
		{
			throw std::exception("Invalid Operation! Find operation cannot be performed due to type mismatch.");
		}
		for (uint32_t i = 0; i < m_size; ++i)
		{
			if (&t_item == m_data.d_RTTI_ptr[i])
			{
				found = true;
				index = i;
				break;
			}
		}
		return (found ? index : m_size);
	}

	const uint32_t& Datum::find(const int32_t& t_item) const
	{
		bool found = false;
		uint32_t index = 0;
		if (m_type == DatumType::UNKNOWN)
		{
			throw std::exception("Invalid Operation! Datum Type isn't set.");
		}
		if (m_type != DatumType::INTEGER)
		{
			throw std::exception("Invalid Operation! Find operation cannot be performed due to type mismatch.");
		}
		for (uint32_t i = 0; i < m_size; ++i)
		{
			if (t_item == m_data.d_int32_t[i])
			{
				found = true;
				index = i;
				break;
			}
		}
		return (found ? index : m_size);
	}

	const uint32_t& Datum::find(const float_t& t_item) const
	{
		bool found = false;
		uint32_t index = 0;
		if (m_type == DatumType::UNKNOWN)
		{
			throw std::exception("Invalid Operation! Datum Type isn't set.");
		}
		if (m_type != DatumType::FLOAT)
		{
			throw std::exception("Invalid Operation! Find operation cannot be performed due to type mismatch.");
		}
		for (uint32_t i = 0; i < m_size; ++i)
		{
			if (t_item == m_data.d_float_t[i])
			{
				found = true;
				index = i;
				break;
			}
		}
		return (found ? index : m_size);
	}

	const uint32_t& Datum::find(const glm::vec4& t_item) const
	{
		bool found = false;
		uint32_t index = 0;
		if (m_type == DatumType::UNKNOWN)
		{
			throw std::exception("Invalid Operation! Datum Type isn't set.");
		}
		if (m_type != DatumType::VECTOR4)
		{
			throw std::exception("Invalid Operation! Find operation cannot be performed due to type mismatch.");
		}
		for (uint32_t i = 0; i < m_size; ++i)
		{
			if (t_item == m_data.d_glm_vec4[i])
			{
				found = true;
				index = i;
				break;
			}
		}
		return (found ? index : m_size);
	}

	const uint32_t& Datum::find(const glm::mat4x4& t_item) const
	{
		bool found = false;
		uint32_t index = 0;
		if (m_type == DatumType::UNKNOWN)
		{
			throw std::exception("Invalid Operation! Datum Type isn't set.");
		}
		if (m_type != DatumType::MATRIX4X4)
		{
			throw std::exception("Invalid Operation! Find operation cannot be performed due to type mismatch.");
		}
		for (uint32_t i = 0; i < m_size; ++i)
		{
			if (t_item == m_data.d_glm_mat4x4[i])
			{
				found = true;
				index = i;
				break;
			}
		}
		return (found ? index : m_size);
	}

	const uint32_t& Datum::find(const std::string& t_item) const
	{
		bool found = false;
		uint32_t index = 0;
		if (m_type == DatumType::UNKNOWN)
		{
			throw std::exception("Invalid Operation! Datum Type isn't set.");
		}
		if (m_type != DatumType::STRING)
		{
			throw std::exception("Invalid Operation! Find operation cannot be performed due to type mismatch.");
		}
		for (uint32_t i = 0; i < m_size; ++i)
		{
			if (t_item == m_data.d_string[i])
			{
				found = true;
				index = i;
				break;
			}
		}
		return (found ? index : m_size);
	}

	const uint32_t& Datum::find(const RTTI& t_item) const
	{
		bool found = false;
		uint32_t index = 0;
		if (m_type == DatumType::UNKNOWN)
		{
			throw std::exception("Invalid Operation! Datum Type isn't set.");
		}
		if (m_type != DatumType::POINTER)
		{
			throw std::exception("Invalid Operation! Find operation cannot be performed due to type mismatch.");
		}
		for (uint32_t i = 0; i < m_size; ++i)
		{
			if (&t_item == m_data.d_RTTI_ptr[i])
			{
				found = true;
				index = i;
				break;
			}
		}
		return (found ? index : m_size);
	}
}