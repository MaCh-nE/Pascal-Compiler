program TestExample;
const
  pi = 3.14159;
  max_value = 100;

var
  x, y, result: real;
  i: integer;

begin
  write('Enter a number:');
  read(x);

  if x > pi then
  begin
    write('x is greater than pi');
  end
  else
  begin
    write('x is not greater than pi');
  end;

  for i := 1 to max_value do
  begin
    y := x * i;
    write('y = ', y:0:2);
  end;

  case i of
    1: write('i is one');
    2: write('
