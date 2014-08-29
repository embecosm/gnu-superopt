
#define HASHTABLE_BUCKETS   (1024*8)


struct hashtable_entry_t {
  word *values;
  int n_values;
  int carry;
  int cost;
  struct hashtable_entry_t *next;
};

typedef struct hashtable_entry_t hashtable_entry;

hashtable_entry hashtable[HASHTABLE_BUCKETS];

void hashtable_init();
void hashtable_insert(word *values, int n_values, int carry, int cost);
int hashtable_find(word *values, int n_values, int carry);
void hashtable_free();

void hashtable_init()
{
  int i;

  for (i = 0; i < HASHTABLE_BUCKETS; ++i)
    {
      hashtable[i].values = NULL;
      hashtable[i].n_values = -1;
      hashtable[i].carry = 0;
      hashtable[i].next = NULL;
    }
}

unsigned hash(word *values, int n_values, int carry)
{
  unsigned v = 0;
  int i;

  for (i = 0; i < n_values; ++i)
    {
      v += values[i] * (i+1) * carry;
    }

  return v % HASHTABLE_BUCKETS;
}

int compare(word *values_1, int n_values_1, int carry_1, word *values_2, int n_values_2, int carry_2)
{
  int i;

  if (n_values_1 != n_values_2 || carry_1 != carry_2)
    return 0;

  return memcmp(values_1, values_2, n_values_1*sizeof(word))==0;
}

void hashtable_insert(word *values, int n_values, int carry, int cost)
{
  unsigned h = hash(values, n_values, carry);
  hashtable_entry *e = &hashtable[h];
  int exists = 0;

  while(1)
    {
      if (compare(values, n_values, carry, e->values, e->n_values, e->carry))
        {
          if (cost > e->cost)
              e->cost = cost;
          return;
        }

      if(!e->next)
        {
          e->next = malloc(sizeof(hashtable_entry));
          e = e->next;
          e->cost = cost;
          e->n_values = n_values;
          e->values = malloc(sizeof(word)*n_values);
          e->carry = carry;
          memcpy(e->values, values, sizeof(word)*n_values);
          e->next = NULL;
          return;
        }

      e = e->next;
    }
}

int hashtable_find(word *values, int n_values, int carry)
{
  unsigned h = hash(values, n_values, carry);
  hashtable_entry *e = &hashtable[h];
  int exists = 0;

  while(1)
    {
      if (compare(values, n_values, carry, e->values, e->n_values, e->carry))
        {
          return e->cost;
        }

      if(!e->next)
        {
          return 0;
        }

      e = e->next;
    }
}
