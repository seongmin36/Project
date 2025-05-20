const inputBox = document.querySelector('#todo_input') as HTMLInputElement;
const doList = document.querySelector('#todo_list') as HTMLUListElement;
const todoForm = document.querySelector('#todo_form') as HTMLFormElement;

// type 정의
type Todo = {
  id: number;
  text: string;
};

// 배열 변수 생성
let todos: Todo[] = [];

// input 공백 제거 함수
const noEmpty = (): string => {
  return inputBox.value.trim();
};

// 할 일 렌더링
const renderTasks = (): void => {
  doList.innerHTML = '';

  todos.forEach((todo: Todo) => {
    const addLi = document.createElement('li');
    addLi.textContent = todo.text;
    addLi.className =
      'd-flex list-group-item list-group-item-action list-group-item-warning';
    doList.appendChild(addLi);

    const xBtn = document.createElement('button');
    xBtn.textContent = 'X';
    xBtn.className = 'btn-close ms-auto';
    addLi.appendChild(xBtn);

    xBtn.addEventListener('click', function () {
      deleteItem(todo);
    });
  });
};

// input data li 추가 함수
const addItem = (): void => {
  const text = noEmpty();

  if (text) {
    pushTodos(text);
  }
};

// todos 배열에 push 하는 함수
const pushTodos = (text: string): void => {
  todos.push({ id: Date.now(), text });
  inputBox.value = '';
  renderTasks();
};

// li data 삭제 함수
const deleteItem = (todo: Todo): void => {
  todos = todos.filter((t): boolean => t.id !== todo.id);
  renderTasks();
};

// form 제출 이벤트 리스너
todoForm.addEventListener('submit', function (e: Event): void {
  e.preventDefault();
  addItem();
});
