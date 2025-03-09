#pragma once
#include <iosfwd>
#include <map>
#include <vector>
#include <mutex>

class Buffer;
class IPrimitive;

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
		 */
		Builder& add(const IPrimitive& primitive);

		/**
		 * Finalizes and constructs the buffer stored by this builder. The resulting buffer will be
		 * registered in the buffer manager, and can be bound and rendered via the graphics engine.
		 * @return The buffer that was created
		 */
		void finish();

	private:

		/**
		 * Buffer name
		 */
		std::string _name;

		/**
		 * Vector containing the data buffered from primitives
		 */
		std::vector<float> _data;

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
	 */
	static Builder startBuffer(const std::string& name);

	/**
	 * Draws a buffer to the current rendering context
	 * @param name The name of the buffer to draw. Must be a buffer that was previously constructed
	 * via this manager.
	 */
	static void draw(const std::string& name);

	/**
	 * Destroys a buffer that is no longer needed. If this buffer was currently bound, it will be
	 * unbound prior to destroying.
	 * @param name The name of the buffer to destroy. Must be a buffer that was previously constructed
	 * via this manager.
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
	 */
	static void addBuffer(Buffer& buffer);

};