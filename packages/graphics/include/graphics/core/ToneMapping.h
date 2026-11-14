#pragma once

/**
 * Per-channel tone curve applied to exposed linear RGB before display conversion.
 * @author Nathaniel Rex
 */
enum class ToneMapping
{
    /** 
     * Pass exposed RGB through, clipping to the display range.
     */
    NONE = 0,

    /**
     * Compress highlights using x / (1 + x).
     */
    REINHARD = 1,

    /**
     * Compress highlights using 1 - exp(-x).
     */
    EXPONENTIAL = 2
};
