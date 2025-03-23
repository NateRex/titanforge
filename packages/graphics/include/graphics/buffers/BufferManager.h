#pragma once
#include <iosfwd>
#include <graphics/primitives/Primitive.h>
#include <map>
#include <vector>
#include <mutex>

class Buffer;

/**
 * The buffer manager is responsible for tracking all buffers that have been created
 * @author Nathaniel Rex
 */
class BufferManager
{
public:

	friend class Engine;
	

	/**
	 * Builder class used to construct a buffer containing the data for one or more primitives.
	 * @author Nathaniel Rex
	 */
	class Builder
	{
	public:

		friend class BufferManager;

		/**
		 * Adds data from a primitive to this buffer
		 * @param primitive Primitive to add
		 * @return This builder instance
		 * @throws IllegalArgumentException if the primitive does not have matching attributes with primitives already
		 * added to this buffer
		 */
		Builder& add(const Primitive& primitive);

		/**
		 * Finalizes and constructs the buffer stored by this builder. The resulting buffer will be
		 * registered in the buffer manager, and can be bound and rendered via the graphics engine.
		 * @return The buffer that was created
		 */
		Buffer* finish();

	private:

		/**
		 * Buffer name
		 */
		std::string _name;

		/**
		 * Attributes shared by all primitives stored in this buffer
		 */
		VertexAttributes _attributes;

		/**
		 * Vector containing the vertex data obtained from primitives
		 */
		std::vector<float> _vertexData;

		/**
		 * Vector containing the index data obtained from primitives
		 */
		std::vector<int> _indexData;

		/**
		 * Constructor
		 * @param name A name to give to the finalized buffer
		 */
		Builder(const std::string& name);

	};


public:

	/**
	 * Starts construction of a new buffer. Once finalized via finish(), this buffer will be capable of
	 * being bound for rendering via this manager.
	 * @param name A unique name to give to the new buffer
	 * @return A buffer builder instance.
	 * @throws IllegalArgumentException If a buffer with that name already exists
	 */
	static Builder startBuffer(const std::string& name);

	/**
	 * Retrieves a buffer previously created by this manager
	 * @param name The name of the buffer. Must be a buffer previously created via this manager
	 * @return The buffer
	 * @throws IllegalArgumnentException If no buffer with that name exists
	 */
	static Buffer* get(const std::string& name);

	/**
	 * Destroys a buffer that is no longer needed. If this buffer was currently bound, it will be
	 * unbound prior to destroying.
	 * @param name The name of the buffer to destroy. Must be a buffer that was previously constructed
	 * via this manager.
	 * @throws IllegalArgumentException If no buffer with that name exists
	 */
	static void destroy(const std::string& name);

private:

	/**
	 * Mutex lock for manipulating static data
	 */
	static std::mutex _MUTEX;

	/**
	 * Map containing all buffers that have been created
	 */
	static std::map<std::string, Buffer> _BUFFERS;

	/**
	 * Constructor
	 */
	BufferManager() = delete;

	/**
	 * Constructor
	 * @param mgr Buffer manager to copy from
	 */
	BufferManager(const BufferManager& mgr) = delete;

	/**
	 * Constructor
	 * @param mgr Buffer manager to copy from
	 */
	BufferManager(BufferManager&& mgr) = delete;

	/**
	 * Initializes the buffer manager
	 */
	static void setup();

	/**
	 * Clears and deletes all buffers held by the buffer manager
	 */
	static void clear();

	/**
	 * Registers a newly constructed builder with this manager. Upon doing so, the buffer
	 * can be bound and rendered via the graphics engine.
	 * @param buffer The buffer to register
	 * @return A pointer to the newly registered buffer
	 */
	static Buffer* addBuffer(Buffer& buffer);

};