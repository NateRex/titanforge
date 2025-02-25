#pragma once

/**
 * Test fixture containing initialization logic to run before each test that requires graphics
 * capabilities.
 * @author Nathaniel Rex
 */
class TestFixture
{
public:
	
	/**
	 * Constructor
	 */
	TestFixture();

	/**
	 * Destructor
	 */
	~TestFixture();

	/**
	 * Code to run before each test
	 */
	void setup();

	/**
	 * Code to run after each test
	 */
	void teardown();
};