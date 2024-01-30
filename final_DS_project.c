#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define size 10
#define size_2 15

typedef struct { // GOOD
  char s_name[30];
  int s_id;
  int s_priority;
} student_info;

typedef struct tag { // GOOD
  char s_name[50];
  int s_id;
  struct tag *next;
} node;

void menu(void) { // GOOD
  printf("\n\n\t\t---------- Course Registration | CSC 5322 ---------\n");
  printf("\n\t\t Please choose one option:\n");
  printf("\n\t\t\t1. Start the Program.\n");
  printf("\n\t\t\t2. Deposit a Special Request.\n");
  printf("\n\t\t\t3. Drop the Course.\n");
  printf("\n\t\t\t4. Print Special Request List (sorted).\n");
  printf("\n\t\t\t5. Special Request Process is OVER\n");
  printf("\n\t\t\t\t-| Your choise please: ");
}

node* add_student_LL(node*head, node*ToAdd){ //GOOD
  node* walker;
  if(head==NULL)
    head=ToAdd;
  else{
      walker=head;
      while (walker->next!=NULL)
          walker=walker->next;
      walker->next=ToAdd;
  }
return(head);
}

void Traversal_test(node*head){  //TEST
  node*walker=head;
  while(walker->next!=NULL){
    printf("Test %d %s\n",walker->s_id,walker->s_name);
    walker=walker->next;
  }
}

node *load_CSC5322(node *head, FILE *infp, int *student_IN_LL) { //Problem: 11 students

  node *new_node;
  int i = 0, L;
  while (!feof(infp)) {
    i++;
    new_node = (node *)malloc(sizeof(node));
    fscanf(infp,"%d", &new_node->s_id);
    fgets(new_node->s_name, 19, infp);
    L = strlen(new_node->s_name);
    new_node->s_name[L - 1] = '\0';
    new_node->next = NULL;
    head = add_student_LL(head, new_node);
  }
  *student_IN_LL = i;
  return head;
}

int max_element_index(student_info max_heap[], int index_1, int index_2) { //GOOD
  if (max_heap[index_1].s_priority > max_heap[index_2].s_priority)
    return index_1;
  else
  return index_2;
}

void swap(student_info *A, student_info *B) { //GOOD
  student_info temp;
  temp = *A;
  *A = *B;
  *B= temp;
}

void heapfiy(student_info max_heap[], int parent_index, int last_index) {//GOOD
  int left_index, right_index, max_index;
  if (parent_index > (last_index - 1) / 2)
    return;
  else {
    left_index = (2 * parent_index) + 1;
    right_index = (2*parent_index) + 2;

    if (right_index > last_index)
      max_index = max_element_index(max_heap, left_index, parent_index);
    else {
      max_index = max_element_index(max_heap, left_index, parent_index);
      max_index = max_element_index(max_heap, right_index, max_index);
    }
    if (max_index != parent_index) {
      swap(&max_heap[max_index], &max_heap[parent_index]);
      heapfiy(max_heap, max_index, last_index);
    }
  }
}

int construct_heap(student_info max_heap[], FILE *infp) { //Some priorities are not correct
  int n_student, last_index, i = 0, L;
  char reason[60];
  char line[60];
  fseek(infp, 0, 0);

  while (!feof(infp)) {
    fgets(max_heap[i].s_name, 19, infp);
    L = strlen(max_heap[i].s_name);
    max_heap[i].s_name[L - 1] = '\0';
    //printf("TEST: %s\n", max_heap[i].s_name);
    fscanf(infp, "%d\n", &max_heap[i].s_id);
    //printf("TEST: %d\n", max_heap[i].s_id);
    fgets(reason, 60, infp);
    //printf("TEST: %s\n", reason);
    if (strncmp(reason, "I want to take this course as an elective", 40) == 0)
      max_heap[i].s_priority = 1;
    else if (strncmp(reason, "The course description seems interesting", 41) ==
             0)
      max_heap[i].s_priority = 2;
    else if (strncmp(reason,
                     "I need a 3 credits course to graduate next semester",
                     51) == 0)
      max_heap[i].s_priority = 3;
    fgets(line, 60, infp);
    i++;
  }
  n_student = i;
  last_index = n_student - 1;
  for (i = (last_index - 1) / 2; i >= 0; i--)
    heapfiy(max_heap, i, last_index);
  return n_student;
}

student_info create_student_toadd(void) { //GOOD
  student_info new_student;
  getchar();
  printf("\n\t\t Enter Name: ");
  gets(new_student.s_name);
  printf("\n\t\t Enter ID: ");
  scanf("%d", &new_student.s_id);
  printf("\n\t\t Choose one of the following reasons: \n");
  printf("\n\t\t\t 1. I want to take this course as an elective.\n");
  printf("\n\t\t\t 2. The course description seems interesting.\n");
  printf("\n\t\t\t 3. I need a 3 credits course to graduate next semester.\n");
  printf("\n\t\t\t Input your choice: ");
  scanf("%d", &new_student.s_priority);
  return new_student;
}

int add_new_student_toheap(student_info max_heap[], student_info student_toadd,int *student_IN_MX) { //GOOD
  int index;
  if (*student_IN_MX == size)
    return 0;
else{
  index = (*student_IN_MX)++;
  max_heap[index] = student_toadd;

  while ((index >=1) && max_heap[index].s_priority > max_heap[(index - 1) / 2].s_priority)
  {
    swap(&max_heap[index], &max_heap[(index - 1) / 2]);
    index = (index - 1) / 2;
  }
  return 1;
  }
}

node* delete_node(node* head, int id, int* status){ //GOOD

    node* walker, *temp;
    *status = 0;

    if(head == NULL)
        *status = -1;
    else if(head->s_id == id){
            temp = head;
            head = head->next;
            free(temp);
            *status = 1;
        }
        else
        {
            walker = head;
            while(walker->next != NULL)
            {
                if(walker->next->s_id == id)
                {
                    temp = walker->next;
                    walker->next = temp->next;
                    free(temp);
                    *status = 1;
                }
                else
                    walker = walker->next;
            }
        }
    return (head);
}

int drop_course(student_info max_heap[], node **head, int id, int *student_IN_MX, char *name_added) {

  int deleting, last_index;
  node *temp;
  *head = delete_node((*head), id, &deleting);
  
  if (deleting == 1) {
    if ((*student_IN_MX) != 0) {
      last_index = (*student_IN_MX) - 1;
      temp = (node *)malloc(sizeof(node));
      strcpy(temp->s_name, max_heap[0].s_name);
      strcpy(name_added, temp->s_name);
      temp->s_id = max_heap[0].s_id;
      temp->next = NULL;

      max_heap[0] = max_heap[last_index--];
      (*student_IN_MX)--;

      heapfiy(max_heap, 0, last_index);

      *head = add_student_LL((*head), temp);
    } else
      strcpy(name_added, "NO");
  }
  return deleting;
}

student_info Delete_from_heap(student_info* max_heap,int* n_students){
    student_info Todelete;
    int root_i=0,child_i,last_i;
    if (*n_students==0)
        printf("Heap is Empty.");
    else
    {
    Todelete=max_heap[root_i];
    max_heap[root_i]=max_heap[--(*n_students)];
    last_i=*n_students-1;
    child_i=(2*root_i)+1;
        while (child_i<=last_i) {
            if (max_heap[child_i].s_priority<max_heap[child_i+1].s_priority)
                child_i++;
            if (max_heap[child_i].s_priority>max_heap[root_i].s_priority)
            {
                swap(&max_heap[child_i],&max_heap[root_i]);
                root_i=child_i;
                child_i=(2*root_i)+1;
            }
            else
                break;
        }
    }
    return Todelete;
}

void print_special_request_list(student_info* max_heap, int n_students_heap) {

  student_info temp[size],deleted;
  int n_students_temp=n_students_heap,i;

  for (i = 0; i < n_students_heap; i++)
    temp[i] = max_heap[i];

  printf("\n\t\t----------------- Heap's Content in Descending Order "
         "-------------------\n");
    
  while (n_students_temp != 0) {
    deleted=Delete_from_heap(temp,&n_students_heap);
    printf("\n\n\t\t Student %d°:", ++i);
    printf("\n\t\t\t Name     : %s", deleted.s_name);
    printf("\n\t\t\t ID       : %d", deleted.s_id);
    printf("\n\t\t\t Priority : %d", deleted.s_priority);
  }
}



void updating_files(student_info max_heap[], node *walker, FILE *infp_1,FILE *infp_2, int student_IN_MX, 
int *Enrollment,int *Waitlisted) {

  int i = 0;

  while (i != size_2) {
    while (walker != NULL) {
      fprintf(infp_1, "%s\n", walker->s_name);
      fprintf(infp_1, "%d\n", walker->s_id);

      if (i == size_2)
        fprintf(infp_1, "------------------------");

      else if (walker->next == NULL && student_IN_MX == 0)
        fprintf(infp_1, "------------------------");

      else
        fprintf(infp_1, "------------------------\n");

      walker = walker->next;
      i++;
    }
    while (student_IN_MX != 0) {
      fprintf(infp_1, "%s\n", max_heap[0].s_name);
      fprintf(infp_1, "%d\n", max_heap[0].s_id);

      if (i == size_2 || student_IN_MX == 1)
        fprintf(infp_1, "------------------------");
      else
        fprintf(infp_1, "------------------------\n");

      max_heap[0] = max_heap[((student_IN_MX--) - 1)];
      heapfiy(max_heap, 0, student_IN_MX - 1);
      i++;

      if (i == size_2)
        break;
    }
    break;
  }
  *Enrollment = i;
  *Waitlisted = 0;

  for (int i = 0; i < student_IN_MX; i++) {
    fprintf(infp_2, "%s\n", max_heap[i].s_name);
    fprintf(infp_2, "%d\n", max_heap[i].s_id);

    if (max_heap[i].s_priority == 1)
      fprintf(infp_2, "I want to take this course as an elective");
    else if (max_heap[i].s_priority == 1)
      fprintf(infp_2, "The course description seems interesting");
    else
      fprintf(infp_2, "I need a 3 credits course to graduate next semester");

    fprintf(infp_2, "------------------------");

    (*Waitlisted)++;
  }
}

int main(void) {

  node *head = NULL;
  student_info max_heap[size];
  int choise, student_IN_LL=0, student_IN_MX=0, adding, droping, Enrollment,
      Waitlisted;
  FILE *infp_1, *infp_2;
  int id;
  char name_added[20];
  student_info student_toadd;

  do {
    menu();
    scanf("%d", &choise);

    switch (choise) {
    case 1:

      infp_1 = fopen("students_data.txt", "r");
      if (infp_1 == NULL)
        printf("\n\t\t !!! students_data.txt file is not FOUND !!!\n");
      else
        head = load_CSC5322(head, infp_1, &student_IN_LL);
      if (student_IN_LL == 0)
        printf("\n\t\t\t ---> No student has been registerd successfully in "
               "the class YET.\n");
      else
        printf("\n\t\t There is %d student registerd in class SO FAR.\n",
               student_IN_LL);
      fclose(infp_1);
      infp_2 = fopen("special_requests.txt", "r");
      if (infp_2 == NULL)
        printf("\n\t\t !!! special_requests.txt file is not FOUND !!!\n");
      else
        student_IN_MX = construct_heap(max_heap, infp_2);

      if (student_IN_MX == 0)
        printf("\n\t\t !!! The MAX HEAP is empty !!!\n");

      else
        printf("\n\t\t\t ---> The MAX HEAP has been constructed successfully "
               "with %d student(s).\n",
               student_IN_MX);
      Traversal_test(head);
      break;

    case 2:

      student_toadd = create_student_toadd();
      adding = add_new_student_toheap(max_heap, student_toadd, &student_IN_MX);

      if (adding == 1)
        printf("\n\t\t %s has been added to the MAX HEAP seccussfully.\n",
               student_toadd.s_name);
      else
        printf("\n\t\t Sorry, the MAX HEAP is full. We coould not add %s.\n",
               student_toadd.s_name);
      break;
    case 3:

      printf("\n\t\t Input ID of the student to delete: ");
      scanf("%d", &id);

      droping = drop_course(max_heap, &head, id, &student_IN_MX, name_added);

      if (droping == -1)
        printf("\n\t\t !!! The LINKED LIST is empty !!! \n");
      else if (droping == 0)
        printf("\n\t\t !!! Student with %d was not found in the LINKED LIST !!! \n",id);
      else {
        if (strcmp(name_added, "NO") == 0)
          printf("\n\t\t The student with ID is %d was droped from the class. "
                "\n\t\t !!! But not replaced "
                 "because the MAX HEAP is empty !!!\n",
                 id);
        else
          printf("\n\t\t The student with ID is %d was droped from the class. "
                 "\n\t\t And replaced by student with ID %s.\n",
                 id, name_added);
      }
      break;

    case 4:
      print_special_request_list(max_heap, student_IN_MX);
      break;

    case 5:
      infp_1 = fopen("students_data.txt", "w");
      infp_2 = fopen("special_requests.txt", "w");
      updating_files(max_heap, head, infp_1, infp_2, student_IN_MX, &Enrollment, &Waitlisted);
      fclose(infp_1);
      fclose(infp_2);
      printf("\n\n\t\t\t Course     : CSC 5322");
      printf("\n\t\t\t Capacity   : %d", size_2);
      printf("\n\t\t\t Enrollment : %d", Enrollment);
      printf("\n\t\t\t Waitlisted : %d\n\n\n", Waitlisted);
      printf(
          "\n\t\t !!! The information in both files has been updeted !!!\n\n");
      printf("\n\t\t The program is over.\n\n\n\n");
      break;

    default:
      printf("\n\t\t !!! wrong choice !!!\n");
    }
  } while (choise != 5);
}

