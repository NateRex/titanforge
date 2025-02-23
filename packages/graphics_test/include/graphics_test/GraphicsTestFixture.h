#pragma once

/**
 * Test fixture containing initialization logic to run before each test that requires graphics
 * capabilities.
 * @author Nathaniel Rex
 */
class GraphicsTestFixture
{
public:
	
	/**
	 * Constructor
	 */
	GraphicsTestFixture();

	/**
	 * Destructore
	 */
	~GraphicsTestFixture();

	/**
	 * Code to run before each test
	 */
	void setup();

	/**
	 * Code to run after each test
	 */
	void teardown();
};