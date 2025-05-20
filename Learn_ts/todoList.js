"use strict";
const inputBox = document.querySelector('#todo_input');
const doList = document.querySelector('#todo_list');
const todoForm = document.querySelector('#todo_form');
// 배열 변수 생성
let todos = [];
// input 공백 제거 함수
const noEmpty = () => {
    return inputBox.value.trim();
};
// 할 일 렌더링
const renderTasks = () => {
    doList.innerHTML = '';
    todos.forEach((todo) => {
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
const addItem = () => {
    const text = noEmpty();
    if (text) {
        pushTodos(text);
    }
};
// todos 배열에 push 하는 함수
const pushTodos = (text) => {
    todos.push({ id: Date.now(), text });
    inputBox.value = '';
    renderTasks();
};
// li data 삭제 함수
const deleteItem = (todo) => {
    todos = todos.filter((t) => t.id !== todo.id);
    renderTasks();
};
// form 제출 이벤트 리스너
todoForm.addEventListener('submit', function (e) {
    e.preventDefault();
    addItem();
});
