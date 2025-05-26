lines = open("rgb_image.mem").read().splitlines()
print(f"Total lines: {len(lines)}")  # Should print: 43200
assert len(lines) == 43200, "ERROR: rgb_image.mem must have 43200 lines for 120x120 RGB"
