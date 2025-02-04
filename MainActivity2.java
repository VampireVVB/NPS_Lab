package com.example.test;

import android.os.Bundle;
import android.widget.TextView;

import androidx.activity.EdgeToEdge;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;

public class MainActivity2 extends AppCompatActivity {

    TextView nameTextView, mobileTextView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EdgeToEdge.enable(this);
        setContentView(R.layout.activity_main2);

        nameTextView = findViewById(R.id.name1);
        mobileTextView = findViewById(R.id.mobile1);

        String username= getIntent().getStringExtra("nameUser");
        String usermobile= getIntent().getStringExtra("mobileUser");

        nameTextView.setText(username);
        mobileTextView.setText(usermobile);
    }
}