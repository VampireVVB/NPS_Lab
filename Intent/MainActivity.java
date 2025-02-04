package com.example.test;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

import androidx.activity.EdgeToEdge;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;

public class MainActivity extends AppCompatActivity {

    private EditText name,mobile;
    private Button submit;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EdgeToEdge.enable(this);
        setContentView(R.layout.activity_main);
        submit = findViewById(R.id.SubmitBtn);
        name = findViewById(R.id.nameEditText);
        mobile = findViewById(R.id.mobileEditText);

        submit.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String n = name.getText().toString();
                String m = mobile.getText().toString();

                Intent intent= new Intent(MainActivity.this, MainActivity2.class);

                intent.putExtra("nameUser",n);
                intent.putExtra("mobileUser",m);

                startActivity(intent);
            }
        });
    }
}