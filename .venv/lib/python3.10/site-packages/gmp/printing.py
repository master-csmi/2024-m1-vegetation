from stringcolor import cs

class printing:
  def __init__(self, txtColor, lnColor):
    self.txtColor = txtColor
    self.lnColor = lnColor
  
  def print(self, *lists):
    lists=list(lists)
    listMax = 0
    for i in lists:
      i.insert(1, " ")
      if len(i)>listMax:
        listMax=len(i)
    for i in lists:
      while len(i)<listMax:
        i.append(" ")
    breakLN = cs("│", self.lnColor)
    max_length = max(len(l) for l in lists)
    padded_lists = []
    for l in lists:
      max_len = max(len(str(x)) for x in l)
      padded_list = [f'{x:<{max_len + 2}}'
                     for x in l] + [''] * (max_length - len(l))
      padded_lists.append(padded_list)
  
    clearLine=""
    lines=""
    for i in range(max_length):
      line = breakLN + f'{breakLN} '.join(
        str(cs(p[i], self.txtColor).bold()) for p in padded_lists) + breakLN
      clearLine = "│" + f'{"│"} '.join(p[i] for p in padded_lists) + "│"
      lines = line if lines == "" else lines + "\n" + line
  
    index = 0
    topLine=cs("┌", self.lnColor)
    bottomLine=cs("└", self.lnColor)
    positions = []
    while index < len(clearLine):
        index = clearLine.find("│", index)
        if index == -1:
            break
        positions.append(index-1)  # You can perform any operation here
        index += 1
  
    for i in range(len(clearLine)-2):
      if i in positions:
        topLine+=cs("┬", self.lnColor)
        bottomLine+=cs("┴", self.lnColor)
      else:
        topLine+=cs("─", self.lnColor)
        bottomLine+=cs("─", self.lnColor)
    topLine+=cs("┐", self.lnColor)
    bottomLine+= cs("┘", self.lnColor)
        
    return f"{topLine}\n{lines}\n{bottomLine}"