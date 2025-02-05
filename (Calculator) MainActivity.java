package com.example.calculatorapp;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    private TextView tvResult;
    private String currentInput = "";
    private String operator = "";
    private double firstNumber = 0;
    private boolean isNewOp = true;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        tvResult = findViewById(R.id.tvResult);
        setNumberClickListeners();
        setOperationClickListeners();
    }

    private void setNumberClickListeners() {
        int[] numberIds = {R.id.btn0, R.id.btn1, R.id.btn2, R.id.btn3, R.id.btn4, R.id.btn5, R.id.btn6, R.id.btn7, R.id.btn8, R.id.btn9};

        View.OnClickListener numberClickListener = new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (isNewOp) {
                    currentInput = "";
                    isNewOp = false;
                }
                Button btn = (Button) v;
                currentInput += btn.getText().toString();
                tvResult.setText(currentInput);
            }
        };

        for (int id : numberIds) {
            findViewById(id).setOnClickListener(numberClickListener);
        }
    }

    private void setOperationClickListeners() {
        int[] operatorIds = {R.id.btnAdd, R.id.btnSubtract, R.id.btnMultiply, R.id.btnDivide};

        View.OnClickListener operationClickListener = new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Button btn = (Button) v;
                operator = btn.getText().toString();
                firstNumber = Double.parseDouble(currentInput);
                isNewOp = true;
            }
        };

        for (int id : operatorIds) {
            findViewById(id).setOnClickListener(operationClickListener);
        }

        findViewById(R.id.btnEquals).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                double secondNumber = Double.parseDouble(currentInput);
                double result = 0;
                switch (operator) {
                    case "+": result = firstNumber + secondNumber; break;
                    case "−": result = firstNumber - secondNumber; break;
                    case "×": result = firstNumber * secondNumber; break;
                    case "÷": result = secondNumber != 0 ? firstNumber / secondNumber : 0; break;
                }
                tvResult.setText(String.valueOf(result));
                isNewOp = true;
            }
        });

        findViewById(R.id.btnClear).setOnClickListener(v -> {
            tvResult.setText("0");
            currentInput = "";
            operator = "";
            firstNumber = 0;
            isNewOp = true;
        });
    }
}
