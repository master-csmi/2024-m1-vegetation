import colorsys
import re

class rainbow:

  def __init__(self, bold):
    self.bold = bold

  def lolcat(self, text):
    text = re.sub('\[[0-9;]+[a-zA-Z]', ' ', str(text))
    hue_range = (0, 360)  # Define the range of hues for the gradient
    saturation = 100  # Saturation remains constant
    value = 100  # Value (brightness) remains constant

    total_chars = len(text)
    hue_step = (hue_range[1] - hue_range[0]) / total_chars

    rainbow_text = ""

    for i, char in enumerate(text):
      if char.isspace():
        rainbow_text += char  # Preserve existing spacing
        continue
      if char == "│":
        rainbow_text += "│"

      hue = hue_range[0] + (i * hue_step)
      rgb = colorsys.hsv_to_rgb(hue / 360, saturation / 100, value / 100)
      if self.bold:  # Check if bold is true
        color_code = "\033[48;2;{};{};{}m".format(
            int(rgb[0] * 255), int(rgb[1] * 255),
            int(rgb[2] * 255))  # Set background color to black
      else:
        # Convert RGB values to ANSI escape code for terminal color
        color_code = "\033[38;2;{};{};{}m".format(int(rgb[0] * 255),
                                                  int(rgb[1] * 255),
                                                  int(rgb[2] * 255))
        rainbow_text += color_code  # Add color for text

      rainbow_text += color_code + char  # Add background color

    # Reset color at the end
    rainbow_text += '\033[0m'

    return rainbow_text
