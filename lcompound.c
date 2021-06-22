

struct test
{
  int a, b;
};

int f(struct test s)
{
  return s.a + s.b;

}

int main(void)
{
  
  f ( (struct test){
      .a = 2, .b = 5
      });

}
