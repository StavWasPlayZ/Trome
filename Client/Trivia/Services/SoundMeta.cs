using System;

namespace Trivia.Services;

public readonly record struct SoundMeta(
    string Path,
    TimeSpan Duration // <- The loop duration.
);